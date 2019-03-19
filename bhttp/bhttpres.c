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

http_resource_t *http_resource_create(void)
{
    http_resource_t *resource;

    resource = (http_resource_t *)malloc(sizeof(http_resource_t));
    if (resource)
    {
        resource->next = NULL;
    }
    return resource;
}

void http_resource_free(http_resource_t *resource)
{
    free(resource);
}

int http_add_resource(
                        http_resource_t       **resources,
                        butil_url_scheme_t      scheme,
                        http_resource_type_t    type,
                        const char             *urlbase,
                        http_credentials_t     *credentials,
                        http_resource_t       **result
                     )
{
    http_resource_t *resource;
    http_resource_t *insertp;
    http_resource_t *previnsertp;

    resource = http_resource_create();
    *result = resource;
    if (! resource)
    {
        HTTP_ERROR("No resource");
        return -1;
    }
    resource->next = NULL;
    resource->scheme = scheme;
    resource->type = type;
    resource->urlbase = urlbase;
    #if HTTP_SUPPORT_AUTH
    if (credentials)
    {
        resource->credentials.user = credentials->user;
        resource->credentials.pass = credentials->pass;
        resource->credentials.type = credentials->type;
    }
    else
    {
        resource->credentials.user = NULL;
        resource->credentials.pass = NULL;
        resource->credentials.type = httpAuthNone;
    }
    #endif
    // insert the resource sorted inverse by length of urlbase
    // so that sub-urls are matched before urls
    //
    if (! *resources)
    {
        *resources = resource;
    }
    else
    {
        size_t baselen = strlen(urlbase);
        bool inserted = false;
        size_t reslen;

        previnsertp = NULL;
        insertp = *resources;

        do
        {
            reslen = strlen(insertp->urlbase);
            if (reslen < baselen)
            {
                resource->next = insertp;
                if (previnsertp == NULL)
                {
                    *resources = resource;
                }
                else
                {
                    previnsertp->next = resource;
                }
                inserted = true;
                break;
            }
            previnsertp = insertp;
            if (insertp->next)
            {
                insertp = insertp->next;
            }
        }
        while (insertp->next);

        if (! inserted)
        {
            insertp->next = resource;
        }
    }
    return 0;
}

int http_file_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    iostream_t *stream = (iostream_t *)client->ctxpriv;
    char path[HTTP_MAX_PATH];
    struct stat fstat;
    time_t modtime;
    bool isupload;
    bool isdownload;
    int result;

    switch (client->method)
    {
    case httpGet:
        isupload = (client->isclient) ? false : true;
        isdownload = !isupload;
        break;
    case httpPut:
        isupload = (client->isclient) ? true: false;
        isdownload = !isupload;
        break;
    case httpPost:
    default:
        isupload = false;
        isdownload = false;
        break;
    }
    switch (cbtype)
    {
    case httpRequest:
        client->ctxpriv = NULL;
        stream = NULL;

        if (count)
        {
            *count = 0;
        }
        if (isupload)
        {
            // concatenate root and path
            if (http_join_path(path, sizeof(path),
                    resource->resource.file_data.root, resource->urlbase, client->path))
            {
                if (client->isclient)
                {
                    HTTP_ERROR("Can't make filename");
                    return -1;
                }
                else
                {
                    http_error_reply(client, 414, "URI overflow", false);
                    return 0;
                }
            }
            // get file size
            result = stat(path, &fstat);
            if (result)
            {
                if (client->isclient)
                {
                    HTTP_ERROR("Can't stat file");
                    return -1;
                }
                else
                {
                    return http_error_reply(client, 404, "Not found", false);
                }
            }
            if ((fstat.st_mode & S_IFMT) != S_IFREG)
            {
                if (client->isclient)
                {
                    HTTP_ERROR("file not a file");
                    return -1;
                }
                else
                {
                    return http_error_reply(client, 405, "Not a file", false);
                }
            }
            if (! client->isclient)
            {
                #ifdef __APPLE__
                modtime = fstat.st_mtime;
                #else
                modtime = fstat.st_mtim.tv_sec;
                #endif

                client->modifiedwhen = modtime;

                if (client->ifmodifiedsince >= client->modifiedwhen && client->modifiedwhen > 0)
                {
                    if (client->ifmodifiedsince >= modtime)
                    {
                        // not an error, just a short-cut
                        //
                        return http_error_reply(client, 304, "Not modified", false);
                    }
                }
            }
            client->out_content_type = butil_content_type_for_file(path);
            client->out_content_length = fstat.st_size;

            // optional: modify xfer encoding
            client->out_transfer_type = httpChunked;

            http_log(3, "upload file %s (%u)\n", path, client->out_content_length);

            stream = iostream_create_reader_from_file(path);
            if (! stream)
            {
                return http_error_reply(client, 500, "Stream", false);
            }
            client->ctxpriv = (void*)stream;
        }
        else if (isdownload)
        {
            // concatenate root and path.
            if (http_join_path(path, sizeof(path),
                    resource->resource.file_data.root, resource->urlbase, client->path))
            {
                if (client->isclient)
                {
                    HTTP_ERROR("Can't make filename");
                    return -1;
                }
                else
                {
                    return http_error_reply(client, 414, "URI overflow", false);
                }
            }
            http_log(3, "download file %s\n", path);

            stream = iostream_create_writer_from_file(path);
            if (! stream)
            {
                if (client->isclient)
                {
                    HTTP_ERROR("Can't make stream");
                    return -1;
                }
                else
                {
                    return http_error_reply(client, 500, "Stream", false);
                }
            }
            client->ctxpriv = (void*)stream;
        }
        break;

    case httpDownloadData:
        if (stream)
        {
            result = stream->write(stream, *data, *count);
            *count = result;
            return (result >= 0) ? 0 : -1;
        }
        HTTP_ERROR("No stream");
        return 1;

    case httpDownloadDone:
        return 0;

    case httpUploadData:
        if (stream)
        {
            result = stream->read(stream, *data, *count);
            *count = result;
            return (result >= 0) ? 0 : -1;
        }
        HTTP_ERROR("No stream");
        return 1;

    case httpComplete:
        if (stream)
        {
            client->ctxpriv = NULL;
            stream->close(stream);
            return 0;
        }
        HTTP_ERROR("No stream");
        return 1;

    default:
        return -1;
    }
    return 0;
}

int http_add_file_resource(
                        http_resource_t   **resources,
                        butil_url_scheme_t  scheme,
                        const char         *urlbase,
                        const char         *root,
                        http_credentials_t *credentials
                       )
{
    http_resource_t *resource;
    int result;

    result = http_add_resource(resources, scheme, httpFileResource, urlbase, credentials, &resource);
    if (! result)
    {
        resource->callback  = http_file_callback;
        resource->priv      = NULL;
        resource->resource.file_data.root = root;
    }
    return result;
}

int http_add_func_resource(
                        http_resource_t   **resources,
                        butil_url_scheme_t  scheme,
                        const char         *urlbase,
                        http_credentials_t *credentials,
                        http_resource_function_t func,
                        void               *priv
                       )
{
    http_resource_t *resource;
    int result;

    result = http_add_resource(resources, scheme, httpFunctionResource, urlbase, credentials, &resource);
    if (! result)
    {
        resource->callback  = func;
        resource->priv      = priv;
    }
    return result;
}

int http_canned_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    size_t offset = (size_t)(intptr_t)client->ctxpriv;
    size_t moved;
    size_t have;

    switch (cbtype)
    {
    case httpRequest:
        client->ctxpriv = (void*)(intptr_t)0;

        switch (client->method)
        {
        case httpGet:
            http_log(7, "get canned %s\n", resource->resource.canned_data.content);
            client->out_content_length = resource->resource.canned_data.count;
            client->out_content_type   = resource->resource.canned_data.content_type;
            break;

        default:
            HTTP_ERROR("Bad method for canned data");
            return -1;
        }
        break;

    case httpDownloadData:
        // pretend like we took it all
        return 0;

    case httpUploadData:
        moved = *count;
        have  = resource->resource.canned_data.count - offset;

        if (have < moved)
        {
            moved = have;
        }
        http_log(3, "move %u of %u at %u\n", moved, have, offset);
        if (moved > 0)
        {
            if (*data)
            {
                memcpy(*data, (uint8_t*)resource->resource.canned_data.content + offset, moved);
            }
            else
            {
                *data = (uint8_t*)resource->resource.canned_data.content + offset;
            }
            offset += moved;
            client->ctxpriv = (void *)(intptr_t)offset;
        }
        *count = moved;
        break;;

    case httpComplete:
        break;

    default:
        return -1;
    }
    return 0;
}

int http_add_canned_resource(
                        http_resource_t   **resources,
                        butil_url_scheme_t  scheme,
                        const char         *urlbase,
                        http_credentials_t *credentials,
                        mime_content_type_t content_type,
                        const uint8_t      *content,
                        const size_t        content_length
                       )
{
    http_resource_t *resource;
    int result;

    result = http_add_resource(resources, scheme, httpCannedResource, urlbase, credentials, &resource);
    if (! result)
    {
        resource->callback = http_canned_callback;
        resource->priv = NULL;
        resource->resource.canned_data.content = content;
        resource->resource.canned_data.content_type = content_type;
        resource->resource.canned_data.count = content_length;
    }
    return result;
}

int http_outbuffer_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    size_t offset = (size_t)(intptr_t)client->ctxpriv;
    size_t moved;
    size_t room;

    switch (cbtype)
    {
    case httpRequest:
        client->ctxpriv = (void*)(intptr_t)0;
        // reset out ring
        client->out.count = 0;
        client->out.head = 0;
        client->out.tail = 0;
        break;

    case httpDownloadData:
        if (! count || ! *count || ! data || ! *data)
        {
            return 0;
        }
        if (client->out.head >= client->out.tail)
        {
            room = client->out.size - client->out.head;
        }
        else
        {
            room = client->out.tail - client->out.head;
        }
        if (room == 0)
        {
            HTTP_ERROR("outbuffer overflow");
            return -1;
        }
        moved = *count;
        if (moved > room)
        {
            moved = room;
        }
        if (moved > 0)
        {
            memcpy(client->out.data + client->out.head, *data, moved);
            client->out.count += moved;
            client->out.head += moved;
            if (client->out.head >= client->out.size)
            {
                client->out.head = 0;
            }
        }
        *count = moved;
        break;

    case httpDownloadDone:
        return 0;

    case httpUploadData:
        if (count)
        {
            *count = 0;
        }
        break;

    case httpComplete:
        break;

    default:
        return -1;
    }
    return 0;
}

#if HTTP_SUPPORT_WEBDAV
int http_add_dav_resource(
                        http_resource_t   **resources,
                        const char         *urlbase,
                        const char         *root,
                        http_credentials_t *credentials
                       )
{
    http_resource_t *resource;
    int result;

    result = http_add_resource(resources, schemeHTTP, httpDavResource, urlbase, credentials, &resource);
    if (! result)
    {
        // by default, dav uses file system for get/put/delete
        resource->callback  = http_file_callback;
        resource->priv      = NULL;
        resource->resource.file_data.root = root;
    }
    return result;
}
#endif

int pattern_match(const char *string, const char *pattern)
{
    if (string == NULL && pattern == NULL)
    {
        return 1;
    }
    if (pattern == NULL)
    {
        return 0;
    }
    if (! strcmp(pattern, "*"))
    {
        return 0;
    }
    while(*string && *pattern)
    {
        if(*pattern == '?')
        {
            pattern++;
            string++;
        }
        else if(*pattern == '*')
        {
            if (! strcmp(pattern, "*"))
            {
                return 0;
            }
            pattern++;
            string++;
            while (*string != '\0' && *string != *pattern)
            {
                string++;
            }
            if (! *string)
            {
                return 1;
            }
            string++;
            pattern++;
        }
        else if(*pattern++ != *string++)
        {
            return 1;
        }
    }
    if(! *pattern)
    {
        return 0;
    }
    if(! *string && ! strcmp(pattern, "*"))
    {
        return 0;
    }
    return 1;
}

static char *http_restype_name(http_resource_type_t type)
{
    switch (type)
    {
    case httpFileResource:      return "file";
    case httpFunctionResource:  return "func";
    case httpCannedResource:    return "cand";
    case httpDavResource:       return "wdav";
    default:                    return "????";
    }
}

http_resource_t *http_match_resource(
                        http_resource_t *resources,
                        const butil_url_scheme_t scheme,
                        const char      *urlpath,
                        http_resource_type_t type
                        )
{
    http_resource_t *resource;
    const char *tnt;

    tnt = http_restype_name(type);

    for (resource = resources; resource; resource = resource->next)
    {
        http_log(5, "Compare resource %s:%s:%s to %s:%s:%s\n",
                butil_scheme_name(resource->scheme),
                http_restype_name(resource->type),
                resource->urlbase,
                butil_scheme_name(scheme),
                tnt,
                urlpath);
        if (resource->type == type && resource->scheme == scheme)
        {
            if (! pattern_match(urlpath, resource->urlbase))
            {
                http_log(5, "Matched resource %s:%s:%s to %s:%s:%s\n",
                        butil_scheme_name(resource->scheme),
                        http_restype_name(resource->type),
                        resource->urlbase,
                        butil_scheme_name(scheme),
                        tnt,
                        urlpath);
                return resource;
            }
        }
    }
    return NULL;
}

http_resource_t *http_find_resource(
                        http_resource_t *resources,
                        const butil_url_scheme_t scheme,
                        const char      *path,
                        http_method_t    method
                        )
{
    http_resource_t *resource;

    resource = http_match_resource(resources, scheme, path, httpFunctionResource);
    if (! resource)
    {
        resource = http_match_resource(resources, scheme, path, httpCannedResource);
    }
    #if HTTP_SUPPORT_WEBDAV
    if (! resource && scheme == schemeHTTP)
    {
        resource = http_match_resource(resources, scheme, path, httpDavResource);
        if (resource)
        {
            // for DAV, use file callback for file operations and a
            // buffering callback for others to put the body of
            // dav requests into clients out buffer ring
            //
            if (
                    method == httpGet
                ||  method == httpPut
                ||  method == httpPost
                ||  method == httpDelete
            )
            {
                resource->callback  = http_file_callback;
            }
            else
            {
                resource->callback  = http_outbuffer_callback;
            }
        }
    }
    #endif
    if (! resource && scheme == schemeHTTP)
    {
        if (
                method == httpGet
            ||  method == httpPut
            ||  method == httpPost
            ||  method == httpDelete
        )
        {
            resource = http_match_resource(resources, scheme, path, httpFileResource);
        }
    }
    if (! resource)
    {
        int sn;

        sn = (int)scheme - BUTIL_FIRST_USER_SCHEME;
        if (sn >= 0 && sn <= BUTIL_NUM_USER_SCHEMES)
        {
            resource = http_match_resource(resources, scheme, path, httpFunctionResource);
            if (! resource)
            {
                resource = http_match_resource(resources, scheme, path, httpCannedResource);
            }
            if (! resource)
            {
                resource = http_match_resource(resources, scheme, path, httpFileResource);
            }
        }
    }
    return resource;
}

int http_check_credentials(
                        http_auth_type_t type,
                        const char *supplied_creds,
                        http_credentials_t *resource_creds
                        )
{
    int len;

    #if ! HTTP_SUPPORT_AUTH
    return 0;
    #endif
    if (! resource_creds)
    {
        return 0;
    }
    if (resource_creds->type == httpAuthNone)
    {
        return 0;
    }
    switch (type)
    {
    case httpAuthNone:
        return -1;

    case httpAuthBasic:
        if (resource_creds->type != httpAuthBasic)
        {
            return -1;
        }
        // creds should be user:pass, so check
        for (len = 0; supplied_creds[len]; len++)
        {
            if (supplied_creds[len] == ':')
            {
                if (len != strlen(resource_creds->user))
                {
                    return -1;
                }
                if (strncmp(supplied_creds, resource_creds->user, len))
                {
                    http_log(3, "User:%s not:%s\n", supplied_creds, resource_creds->user);
                    return -1;
                }
                if (strcmp(supplied_creds + len + 1, resource_creds->pass))
                {
                    http_log(3, "Pass:%s not:%s\n", supplied_creds + len + 1, resource_creds->pass);
                    return -1;
                }
                return 0;
            }
        }
        return -1;

    case httpAuthDigest:
        return -1;

    default:
        break;
    }
    return -1;
}

