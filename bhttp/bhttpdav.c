/*
 * Copyright 2019 Brian Dodge
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "bhttp.h"

static const char *s_multistatus_header =
    "\r\n%04X\r\n"
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<d:multistatus xmlns:d=\"DAV:\">\n";

static const char *s_multistatus_footer =
    "\r\n%04X\r\n"
    "</d:multistatus>\n";

static const char *s_directory_entry =
    "\r\n0000\r\n"
    "<d:response>\n"
    "  <d:href>%s%s</d:href>\n"   // url to directory
    "  <d:propstat>\n"
    "    <d:status>HTTP/1.1 200 OK</d:status>\n"
    "    <d:prop>\n"
    "      <d:resourcetype><d:collection/></d:resourcetype>\n"
    "      <d:getcontenttype>httpd/unix-directory</d:getcontenttype>\n"
    "%s" // filled with quota properties but only if asked for
    "      <d:supportedlock>\n"
    "        <d:lockentry>\n"
    "          <d:lockscope><d:exclusive/></d:lockscope>\n"
    "          <d:locktype><d:write/></d:locktype>\n"
    "        </d:lockentry>\n"
    "        <d:lockentry>\n"
    "          <d:lockscope><d:shared/></d:lockscope>\n"
    "          <d:locktype><d:write/></d:locktype>\n"
    "        </d:lockentry>\n"
    "      </d:supportedlock>\n"
    "%s"                        // lock info
    "    </d:prop>\n"
    "  </d:propstat>\n"
    "</d:response>\n";

static const char *s_file_entry =
    "\r\n0000\r\n"
    "<d:response>\n"
    "  <d:href>%s%s</d:href>\n"   // url to file
    "  <d:propstat>\n"
    "    <d:status>HTTP/1.1 200 OK</d:status>\n"
    "    <d:prop>\n"
    "      <d:resourcetype/>\n"
    "      <d:displayname>%s</d:displayname>\n"             // friendly name of item
    "      <d:getcontentlength>%d</d:getcontentlength>\n"   // size of item
    "      <d:getcontenttype>%s</d:getcontenttype>\n"       // mime type of item
    "      <d:creationdate>%s</d:creationdate>\n"           // creation date
    "      <d:getlastmodified>%s</d:getlastmodified>\n"     // last mod time
    "%s"                        // lock info
    "    </d:prop>\n"
    "  </d:propstat>\n"
    "</d:response>\n";

static const char *s_no_such_file_entry =
    "\r\n0000\r\n"
    "<d:response>\n"
    "  <d:href>%s</d:href>\n"   // url to file
    "  <d:propstat>\n"
    "    <d:status>HTTP/1.1 400 Not-Found</d:status>\n"
    "    <d:prop>\n"
    "      <d:getcontentlength>0</d:getcontentlength>\n"
    "     <d:resourcetype/>\n"
    "    </d:prop>\n"
    "  </d:propstat>\n"
    "</d:response>\n";

typedef struct
{
    char        path[HTTP_MAX_PATH * 2];
    bool        isdir;
    size_t      size;
    time_t   create_time;
    time_t   mod_time;
}
webdav_file_info_t;

#ifdef IOS_HACK
extern DIR *bridgeOpenDir(const char *);
struct dirent *bridgeReadDir(DIR *d);
void bridgeCloseDir(DIR *d);
#define opendir bridgeOpenDir
#define readdir bridgeReadDir
#define closedir bridgeCloseDir
#endif

void http_webdav_findstate_init(http_client_t *client)
{
    int i;

    for (i = 0; i < HTTP_MAX_WEBDAV_DEPTH; i++)
    {
        if (client->dav_find_state[client->dav_depth_at].d)
        {
            HTTP_ERROR("Leftover dir state");
            closedir(client->dav_find_state[client->dav_depth_at].d);
            client->dav_find_state[client->dav_depth_at].d = NULL;
        }
        client->dav_find_state[i].pathpos = 0;
    }
    if (client->resource)
    {
        size_t len = strlen(client->resource->resource.file_data.root);

        if (len > 0)
        {
            len--;
        }
        snprintf(client->dav_findpath, sizeof(client->dav_findpath), "%s%s",
                client->resource->resource.file_data.root,
                (client->resource->resource.file_data.root[0] == '/') ? "" : "/");
    }
    else
    {
        strcpy(client->dav_findpath, "./");
    }
    client->dav_find_state[0].pathpos = strlen(client->dav_findpath);
    client->dav_depth_at = -1;
}

static int webdav_push_dir(http_client_t *client, const char *dirpath)
{
    int pathpos;
    int pathlen;

    if (client->dav_depth_at >= (HTTP_MAX_WEBDAV_DEPTH - 1))
    {
        HTTP_ERROR("DAV depth exceeded");
        return -1;
    }
    // ensure there is room in findpath
    pathpos = strlen(client->dav_findpath);
    for (pathlen = 0; pathlen < pathpos; pathlen++)
    {
        if (! *dirpath)
        {
            break;
        }
        if (*dirpath != client->dav_findpath[pathlen])
        {
            break;
        }
        dirpath++;
    }
    if (pathpos > (sizeof(client->dav_findpath) - strlen(dirpath) - 4))
    {
        HTTP_ERROR("Path overflow");
        return -1;
    }
    // push
    client->dav_depth_at++;

    if (pathpos == 0)
    {
        HTTP_ERROR("No root for find");
        client->dav_findpath[pathpos++] = '/';
    }
    // ensure path ends with path terminator
    if (client->dav_findpath[pathpos - 1] != '/')
    {
        client->dav_findpath[pathpos++] = '/';
    }
    // remember starting position in state path
    client->dav_find_state[client->dav_depth_at].pathpos = pathpos;

    // append dir path to find path, already know it will fit
    snprintf(client->dav_findpath + pathpos, sizeof(client->dav_findpath) - pathpos,
            "%s", (dirpath[0] == '/') ? dirpath + 1 : dirpath);

    // ensure end of findpath has path char
    pathlen = strlen(client->dav_findpath);
    if (client->dav_findpath[pathlen - 1] != '/')
    {
        client->dav_findpath[pathlen++] = '/';
    }
    // open the directory at this path
    client->dav_find_state[client->dav_depth_at].d = opendir(client->dav_findpath);
    if (! client->dav_find_state[client->dav_depth_at].d)
    {
        http_log(1, "No such dir: %s\n", client->dav_findpath);
        client->dav_findpath[client->dav_find_state[client->dav_depth_at].pathpos] = '\0';
        client->dav_depth_at--;
        return -1;
    }
    return 0;
}

static int webdav_pop_dir(http_client_t *client)
{
    if (client->dav_depth_at < 0)
    {
        HTTP_ERROR("DAV stack underflow");
        return -1;
    }
    if (client->dav_find_state[client->dav_depth_at].d)
    {
        closedir(client->dav_find_state[client->dav_depth_at].d);
        client->dav_find_state[client->dav_depth_at].d = NULL;
    }
    // restore path to previous length
    client->dav_findpath[client->dav_find_state[client->dav_depth_at].pathpos] = '\0';

    // pop
    client->dav_depth_at--;
    return 0;
}

static int webdav_file_delete(const char *path)
{
    return unlink(path);;
}

static int webdav_file_move(const char *from, const char *to)
{
    return rename(from, to);
}

static int webdav_file_mkdir(const char *path)
{
    return mkdir(path
            #ifndef _WIN32
                , HTTP_WEBDAV_PERMISSION_MASK
            #endif
            );
}

static int webdav_file_rmdir(const char *path)
{
    return rmdir(path);
}

static int webdav_file_info(webdav_file_info_t *info)
{
    struct stat fstat;
    int result;

    result = stat(info->path, &fstat);
    if (result)
    {
        http_log(2, "No such path: %s\n", info->path);
        return -1;
    }
    info->isdir         = ((fstat.st_mode & S_IFMT) == S_IFDIR) ? true : false;
    info->size          = fstat.st_size;
    #ifdef __APPLE__
    info->create_time   = fstat.st_ctime;
    info->mod_time      = fstat.st_mtime;
    #else
    info->create_time   = fstat.st_ctim.tv_sec;
    info->mod_time      = fstat.st_mtim.tv_sec;
    #endif
    return 0;
}

static int webdav_get_dir_entry(http_client_t *client, webdav_file_info_t *info)
{
    if (client->dav_depth_at < 0 || client->dav_depth_at >= HTTP_MAX_WEBDAV_DEPTH)
    {
        HTTP_ERROR("DAV stack error");
        return -1;
    }
    if (client->dav_find_state[client->dav_depth_at].d)
    {
        struct dirent *dir;

        do
        {
            dir = readdir(client->dav_find_state[client->dav_depth_at].d);
            if (! dir)
            {
                return 1;
            }
        }
        while (dir->d_name[0] == '.');

        snprintf(info->path, sizeof(info->path), "%s%s", client->dav_findpath, dir->d_name);
        return webdav_file_info(info);
    }
    else
    {
        return -1;
    }
}

static int http_webdav_add_dir_entry(http_client_t *client, webdav_file_info_t *info)
{
    const char *purl;
    const char *pdroot;
    int result;
    int room;
    int len;

    // insist buffer is normalized
    //
    if (client->out.tail != 0)
    {
        HTTP_ERROR("Un-normalized ring");
        return -1;
    }
    room = client->out.size - client->out.count;

    // point to path component in info past root
    len = client->dav_find_state[0].pathpos;
    if (len > 0)
    {
        purl = info->path + len - 1;
    }
    else
    {
        purl = "/";
    }
    if (client->resource && client->resource->urlbase && client->resource->urlbase[0])
    {
        pdroot = client->resource->urlbase;
    }
    else
    {
        pdroot = "";
    }
    // format xml
    //
    len = snprintf((char*)client->out.data + client->out.head, room,
                        s_directory_entry, pdroot, purl, "", "");
    if (len < 0 || len >= room)
    {
        HTTP_ERROR("dir entry len");
        return len;
    }
    // back annotate chunk count
    snprintf((char*)client->out.data + client->out.head + 2, 6, "%04X", len - 8);
    client->out.data[client->out.head + 6] = '\r';

    client->out.head += len;
    client->out.count += len;
    return 0;
}

static int http_webdav_add_file_entry(http_client_t *client, webdav_file_info_t *info)
{
    const char *purl;
    const char *pdroot;
    const char *pname;
    const char *pmime;
    char cdate[64];
    char mdate[64];
    int result;
    int room;
    int len;

    // insist buffer is normalized, it should have been done in header formatting
    //
    // insist buffer is normalized
    //
    if (client->out.tail != 0)
    {
        HTTP_ERROR("Un-normalized ring");
        return -1;
    }
    room = client->out.size - client->out.count;

    // point to path component in info past root
    len = client->dav_find_state[0].pathpos;
    if (len > 0)
    {
        purl = info->path + len - 1;
    }
    else
    {
        purl = "/";
    }
    if (client->resource && client->resource->urlbase && client->resource->urlbase[0])
    {
        pdroot = client->resource->urlbase;
    }
    else
    {
        pdroot = "";
    }
    // by default, name is last component of path
    pname = purl + strlen(purl);
    while (pname >= purl)
    {
        if (*--pname == '/')
        {
            break;
        }
    }
    pname++;

    // get mime type if any
    pmime = http_mime_string_for_file(pname);

    // format times to strings
    http_time_to_rfc2616_date(info->mod_time, mdate, sizeof(mdate));
    http_time_to_rfc2616_date(info->create_time, cdate, sizeof(mdate));

    // format xml
    //
    len = snprintf((char *)client->out.data + client->out.head, room,
                        s_file_entry, pdroot, purl, pname, info->size, pmime, cdate, mdate, "");
    if (len < 0 || len >= room)
    {
        HTTP_ERROR("file entry len");
        return -1;
    }
    // back annotate chunk count
    snprintf((char*)(client->out.data + client->out.head + 2), 6, "%04X", len - 8);
    client->out.data[client->out.head + 6] = '\r';

    client->out.head += len;
    client->out.count += len;
    return 0;
}

static int http_webdav_add_contents_of_entry(http_client_t *client, const char *path)
{
    webdav_file_info_t info;
    int result;

    result = webdav_push_dir(client, path);
    return result;
}

static int http_webdav_add_boiler(http_client_t *client, const char *boiler)
{
    int result;
    int room;
    int len;

    // first make sure output buffer is rotated left
    // to allow maximum room (it really is already most likely)
    //
    iostream_normalize_ring(&client->out, NULL);

    // make sure there is room for the boileplate
    //
    len = strlen(boiler);
    room = client->out.size - client->out.count;
    if (room < (len + 16))
    {
        // can't happen
        HTTP_ERROR("No room for header");
        return -1;
    }
    len = snprintf((char *)(client->out.data + client->out.head),
                    client->out.size - client->out.count - 1, boiler, len - 8);
    if (len < 0)
    {
        return len;
    }
    client->out.head += len;
    client->out.count += len;
    return 0;
}

int http_webdav_find_slice(http_client_t *client)
{
    webdav_file_info_t info;
    int result;

    if (client->dav_depth_at < 0)
    {
        http_log(4, "DAV complete\n");

        // append the multi-status reply footer (xml document footer)
        result = http_webdav_add_boiler(client, s_multistatus_footer);
        if (result)
        {
            return result;
        }
        // and a 0 count
        result = http_append_reply(client, "\r\n0\r\n");
        if (result)
        {
            return result;
        }
        // finish sending out the data and end xfer
        //
        result = http_send_out_data(client, httpSendReply, httpKeepAlive);
        return result;
    }
    // insist buffer is normalized to there's no wrapping around head
    //
    iostream_normalize_ring(&client->out, NULL);

    // get next directory entry
    //
    result = webdav_get_dir_entry(client, &info);
    if (result)
    {
        // ran out? if can pop, pop, else end
        //
        if (client->dav_depth_at >= 0)
        {
            return webdav_pop_dir(client);
        }
    }
    if (info.isdir)
    {
        result = http_webdav_add_dir_entry(client, &info);
    }
    else
    {
        result = http_webdav_add_file_entry(client, &info);
    }
    if (! result)
    {
        // incrementally send out this data
        //
        result = http_send_out_data(client, httpSendReply, httpPropFindEnumerate);
    }
    else
    {
        http_log(1, "Error with path:%s\n", info.path);
    }
    return result;
}

int http_webdav_request(http_client_t *client)
{
    int result = 0;
    webdav_file_info_t info, destinfo;
    size_t len;

    http_log(5, "DAV %s %d:%d %s\n", http_method_name(client->method),
            client->dav_depth_in, client->dav_depth_at, client->path);

    switch (client->method)
    {
    case httpPropFind:
        result = http_begin_reply(client, 207, "Multi-Status");
        result |= http_append_reply(client, "Content-Type: text/xml");
        result |= http_append_reply(client, "Transfer-Encoding: chunked");
        result |= http_append_connection_to_reply(client, false);
        if (result)
        {
            return result;
        }
        if (! client->resource)
        {
            // already checked before call here, but..
            HTTP_ERROR("No resource");
            return -1;
        }
        // cleanup find state stack (not expecting issue, but sanity/safe to do)
        //
        http_webdav_findstate_init(client);

        // prepend a multi-status reply header (xml document header)
        //
        result = http_webdav_add_boiler(client, s_multistatus_header);
        if (result)
        {
            return result;
        }
        result = http_join_path(info.path, sizeof(info.path),
                    client->resource->resource.file_data.root,
                    client->resource->urlbase, client->path);
        if (result)
        {
            return result;
        }
        // get info about this file/dir
        //
        result = webdav_file_info(&info);
        if (result)
        {
            result = http_error_reply(client, 404, "Not Found", false);
            return result;
        }
        if (! client->dav_no_root)
        {
            // add the root directory or file entry at path
            //
            if (info.isdir)
            {
                result = http_webdav_add_dir_entry(client, &info);
            }
            else
            {
                result = http_webdav_add_file_entry(client, &info);
            }
        }
        if (info.isdir && (client->dav_depth_in > 0))
        {
            // start iterating directories
            //
            result = http_webdav_add_contents_of_entry(client, info.path);
            if (result)
            {
                return result;
            }
        }
        if (result)
        {
            return result;
        }
        // start sending out the data, will get called back to slice function to
        // complete the operation
        //
        result = http_send_out_data(client, httpSendReply, httpPropFindEnumerate);
        break;

    case httpDelete:
    case httpMkCol:
    case httpCopy:
    case httpMove:
        result = http_join_path(info.path, sizeof(info.path),
                    client->resource->resource.file_data.root,
                    client->resource->urlbase, client->path);
        if (result)
        {
            return result;
        }
        result = webdav_file_info(&info);

        switch (client->method)
        {
        case httpDelete:
            if (result)
            {
                return http_error_reply(client, 404, "Not found", false);
            }
            if (info.isdir)
            {
                result = webdav_file_rmdir(info.path);
                if (result)
                {
                    return http_error_reply(client, 409, "Not Empty", false);
                }
            }
            else
            {
                result = webdav_file_delete(info.path);
                if (result)
                {
                    return http_error_reply(client, 500, "Couldn't Delete", false);
                }
            }
            return http_quick_reply(client, 200, "Deleted", false);

        case httpMkCol:
            if (! result)
            {
                return http_error_reply(client, 409, "Already Exists", false);
            }
            result = webdav_file_mkdir(info.path);
            if (result)
            {
                return http_error_reply(client, 409, "Can't create", false);
            }
            return http_quick_reply(client, 201, "Created", false);

        case httpCopy:
            if (result)
            {
                return http_error_reply(client, 404, "Not found", false);
            }
            return http_error_reply(client, 409, "No Copying", false);

        case httpMove:
            if (result)
            {
                return http_error_reply(client, 404, "Not found", false);
            }
            // for destination path. destination header was stuffed into client findpath
            result = http_join_path(destinfo.path, sizeof(destinfo.path),
                        client->resource->resource.file_data.root,
                        client->resource->urlbase, client->dav_findpath);
            if (result)
            {
                return result;
            }
            result = webdav_file_move(info.path, destinfo.path);
            if (result)
            {
                return http_error_reply(client, 409, "Can't move", false);
            }
            return http_quick_reply(client, 201, "Moved", false);

        default:
            return -1;
        }
        break;

    case httpLock:
    case httpUnlock:
    case httpPropPatch:
    default:
        http_log(1, "Bad DAV request: %s\n", http_method_name(client->method));
        return http_error_reply(client, 405, "Bad Request", false);
    }
    return result;
}

