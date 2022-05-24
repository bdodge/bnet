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

int http_ncasecmp(const char *haystack, const char *needle)
{
    size_t len = strlen(needle);

    return strncasecmp(haystack, needle, len);
}

const butil_url_scheme_t http_scheme_base(const butil_url_scheme_t scheme)
{
    switch (scheme)
    {
    case schemeHTTPS:   return schemeHTTP;
    #if HTTP_SUPPORT_WEBSOCKET
    case schemeWS:      return schemeHTTP;
    case schemeWSS:     return schemeHTTP;
    #endif
    default:            return scheme;
    }
}

const char *http_scheme_base_name(const butil_url_scheme_t scheme)
{
    static char scheme_name_buffer[HTTP_MAX_SCHEME];
    char *pn;

    snprintf(scheme_name_buffer, sizeof(scheme_name_buffer), "%s",
            butil_scheme_name(http_scheme_base(scheme)));
    for (pn = scheme_name_buffer; *pn; pn++)
    {
        if (*pn >= 'a' && *pn <= 'z')
        {
            *pn -= ('a' - 'A');
        }
    }
    return scheme_name_buffer;
}

http_user_method_t s_user_methods[HTTP_NUM_USER_METHODS] =
{
    { "", NULL, NULL }
};

const char *http_method_name(http_method_t method)
{
    int meth;

    switch (method)
    {
    case httpOptions:   return "OPTIONS";
    case httpGet:       return "GET";
    case httpHead:      return "HEAD";
    case httpPost:      return "POST";
    case httpPut:       return "PUT";
    case httpDelete:    return "DELETE";
    case httpTrace:     return "TRACE";
    case httpConnect:   return "CONNECT";
#if HTTP_SUPPORT_WEBDAV
    case httpPropFind:  return "PROPFIND";
    case httpPropPatch: return "PROPPATCH";
    case httpCopy:      return "COPY";
    case httpMkCol:     return "MKCOL";
    case httpMove:      return "MOVE";
    case httpLock:      return "LOCK";
    case httpUnlock:    return "UNLOCK";
#endif
    default:
        meth = (int)method - HTTP_FIRST_USER_METHOD;
        if (meth >= 0 && meth < HTTP_NUM_USER_METHODS)
        {
            if (s_user_methods[meth].name[0])
            {
                return s_user_methods[meth].name;
            }
        }
    }
    return "- bad method -";
}

int http_method_from_name(const char *name, http_method_t *method)
{
    int meth;

    if (! http_ncasecmp(name, "OPTIONS"))
    {
        *method = httpOptions;
        return 0;
    }
    if (! http_ncasecmp(name, "GET"))
    {
        *method = httpGet;
        return 0;
    }
    if (! http_ncasecmp(name, "HEAD"))
    {
        *method = httpHead;
        return 0;
    }
    if (! http_ncasecmp(name, "POST"))
    {
        *method = httpPost;
        return 0;
    }
    if (! http_ncasecmp(name, "PUT"))
    {
        *method = httpPut;
        return 0;
    }
    if (! http_ncasecmp(name, "DELETE"))
    {
        *method = httpDelete;
        return 0;
    }
    if (! http_ncasecmp(name, "TRACE"))
    {
        *method = httpTrace;
        return 0;
    }
    if (! http_ncasecmp(name, "CONNECT"))
    {
        *method = httpConnect;
        return 0;
    }
#if HTTP_SUPPORT_WEBDAV
    if (! http_ncasecmp(name, "PROPFIND"))
    {
        *method = httpPropFind;
        return 0;
    }
    if (! http_ncasecmp(name, "PROPPATCH"))
    {
        *method = httpPropPatch;
        return 0;
    }
    if (! http_ncasecmp(name, "COPY"))
    {
        *method = httpCopy;
        return 0;
    }
    if (! http_ncasecmp(name, "MKCOL"))
    {
        *method = httpMkCol;
        return 0;
    }
    if (! http_ncasecmp(name, "MOVE"))
    {
        *method = httpMove;
        return 0;
    }
    if (! http_ncasecmp(name, "LOCK"))
    {
        *method = httpLock;
        return 0;
    }
    if (! http_ncasecmp(name, "UNLOCK"))
    {
        *method = httpUnlock;
        return 0;
    }
#endif
    for (meth = 0; meth < HTTP_NUM_USER_METHODS; meth++)
    {
        if (s_user_methods[meth].name[0])
        {
            http_log(5, "Compare method=%s= to =%s=\n",
                    name, s_user_methods[meth].name);
            if (! http_ncasecmp(name, s_user_methods[meth].name))
            {
                *method = (http_method_t)(HTTP_FIRST_USER_METHOD + meth);
                return 0;
            }
        }
        else
        {
            break;
        }
    }
    *method = httpUnsupported;
    return -1;
}

int http_register_method(
                        const char *name,
                        http_method_callback_t callback,
                        void *priv,
                        http_method_t *method
                        )
{
    int meth;

    for (meth = 0; meth < HTTP_NUM_USER_METHODS; meth++)
    {
        if (! s_user_methods[meth].name[0])
        {
            strncpy(s_user_methods[meth].name, name, HTTP_MAX_METHOD_NAME - 1);
            s_user_methods[meth].name[HTTP_MAX_METHOD_NAME - 1] = '\0';
            s_user_methods[meth].callback = callback;
            s_user_methods[meth].priv = priv;
            if (method)
            {
                *method = HTTP_FIRST_USER_METHOD + meth;
            }
            return 0;
        }
    }
    butil_log(0, "No user method slots left\n");
    if (method)
    {
        *method = HTTP_FIRST_USER_METHOD + meth;
    }
    return -1;
}

const http_user_method_t *http_get_user_method(http_method_t method)
{
    http_user_method_t *handler;
    int meth;

    meth = (int)method - HTTP_FIRST_USER_METHOD;
    if (meth < 0 || meth >= HTTP_NUM_USER_METHODS)
    {
        return NULL;
    }
    handler = &s_user_methods[meth];
    if (handler->name[0] == '\0')
    {
        return NULL;
    }
    return handler;
}

int http_make_user_method_callback(
                            http_client_t *client,
                            http_method_t method,
                            const char *data
                            )
{
    const http_user_method_t *handler;

    handler = http_get_user_method(method);
    if (! handler)
    {
        return -1;
    }
    if (! handler->callback)
    {
        // this is ok, methods are allowed to ignore us
        return 0;
    }
    return handler->callback(client, method, data, handler->priv);
}

static inline uint8_t byte_from_hex(char x)
{
    if (x >= '0' && x <= '9')
    {
        return x - '0';
    }
    if (x >= 'A' && x <= 'F')
    {
        return x - 'A' + 10;
    }
    if (x >= 'a' && x <= 'f')
    {
        return x - 'a' + 10;
    }
    HTTP_ERROR("Bad hex digit");
    return 0;
}

int http_join_path(char *path, size_t room, const char *root, const char *base, const char *file)
{
    size_t i;
    size_t filepos;

    if (! path || ! root || ! root[0])
    {
        HTTP_ERROR("No root");
        return -1;
    }
    if (! file)
    {
        file = "";
    }
    // remove any path delimter in front of filename
    //
    while (file[0] == '/' || file[0] == '\\')
    {
        file++;
    }
    // put root path into result
    //
    for (i = 0; i < room && root[i]; i++)
    {
        if (root[i] == '/' || path[i] == '\\')
        {
            path[i] = HTTP_LOCAL_PATH_CHAR;
        }
        else
        {
            path[i] = root[i];
        }
    }
    if (i >= (room - 4) || i == 0)
    {
        HTTP_ERROR("Path overflow");
        return -1;
    }
    // ensure the path ends with a delimiter
    //
    if (path[i - 1] != HTTP_LOCAL_PATH_CHAR)
    {
        path[i++] = HTTP_LOCAL_PATH_CHAR;
    }
    // remember spot where file portion of path goes
    //
    filepos = i;

    // url-decode file in-site during the copy to path operation
    //
    while ((i < room) && *file)
    {
        if (*file == '/' || *file == '\\')
        {
            file++;
            path[i++] = HTTP_LOCAL_PATH_CHAR;
        }
        else if (*file == '%')
        {
            file++;
            if (file[0] && file[1])
            {
                uint8_t msn, lsn;

                msn = byte_from_hex(file[0]);
                lsn = byte_from_hex(file[1]);
                file += 2;

                path[i++] = (char)((msn << 4) | lsn);
            }
            else
            {
                break;
            }
        }
        else if (*file == '+')
        {
            file++;
            path[i++] = ' ';
        }
        else if (*file == '?')
        {
            break;
        }
        else
        {
            path[i++] = *file++;
        }
    }
    if (i >= room)
    {
        HTTP_ERROR("Path overflow");
        return -1;
    }
    path[i] = '\0';

    if (base && base[0])
    {
        size_t baselen = strlen(base);
        size_t namelen = strlen(path + filepos);
        size_t remlen;

        if (base[0] == '/' || base[0] == '\\')
        {
            base++;
            baselen--;
        }
        if (namelen >= baselen)
        {
            if (! strncmp(path + filepos, base, baselen))
            {
                remlen = namelen - baselen;
                if (remlen > 0)
                {
                    if (path[filepos + baselen] == HTTP_LOCAL_PATH_CHAR)
                    {
                        baselen++;
                    }
                    memmove(path + filepos, path + filepos + baselen, remlen);
                }
                path[filepos + remlen] = '\0';
            }
        }
    }
    return 0;
}

int http_auth_string_to_type(const char *auth_str, http_auth_type_t *auth_type)
{
    if (! http_ncasecmp(auth_str, "basic"))
    {
        *auth_type = httpAuthBasic;
    }
    else if (! http_ncasecmp(auth_str, "bearer"))
    {
        *auth_type = httpAuthBearer;
    }
    else if (! http_ncasecmp(auth_str, "digest"))
    {
        *auth_type = httpAuthDigest;
    }
    else if (! http_ncasecmp(auth_str, "mutual"))
    {
        *auth_type = httpAuthMutual;
    }
    else if (! http_ncasecmp(auth_str, "oauth"))
    {
        *auth_type = httpAuthOAuth;
    }
    else
    {
        *auth_type = httpAuthNone;
        return -1;
    }
    return 0;
}

const char *http_auth_type_to_string(http_auth_type_t auth_type)
{
    switch (auth_type)
    {
    case httpAuthNone:      return "None";
    case httpAuthBasic:     return "Basic";
    case httpAuthBearer:    return "Bearer";
    case httpAuthDigest:    return "Digest";
    case httpAuthMutual:    return "Mutual";
    case httpAuthOAuth:     return "OAuth";
    default:                return "???";
    }
}

int http_generate_boundary(char *boundary, size_t nboundary)
{
    char stamp[64];
    char randbuf[64];
    time_t now;

    if (! boundary || (nboundary < 32))
    {
        return -1;
    }
    http_time(&now);
    ctime_r(&now, stamp);
    butil_base64_encode(randbuf, sizeof(randbuf), (uint8_t*)stamp, strlen(stamp), false, false);
    randbuf[28] = '\0';
    snprintf(boundary, nboundary, "----%s", randbuf);
    boundary[nboundary - 1] = '\0';
    return 0;
}

