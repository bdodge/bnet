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
    return butil_scheme_name(http_scheme_base(scheme));
}

const char *http_method_name(http_method_t method)
{
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
#if HTTP_SUPPORT_SIP
    case httpInvite:    return "INVITE";
    case httpAck:       return "ACK";
    case httpPrack:     return "PRACK";
    case httpCancel:    return "CANCEL";
    case httpUpdate:    return "UPDATE";
    case httpInfo:      return "INFO";
    case httpSubscribe: return "SUBSCRIBE";
    case httpNotify:    return "NOTIFY";
    case httpRefer:     return "REFER";
    case httpMessage:   return "MESSAGE";
    case httpRegister:  return "REGISTER";
    case httpBye:       return "BYE";
#endif
    default:            return "- bad method -";
    }
}

int http_method_from_name(const char *name, http_method_t *method)
{
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
#if HTTP_SUPPORT_SIP
    if (! http_ncasecmp(name, "INVITE"))
    {
        *method = httpInvite;
        return 0;
    }
    if (! http_ncasecmp(name, "ACK"))
    {
        *method = httpAck;
        return 0;
    }
    if (! http_ncasecmp(name, "PRACK"))
    {
        *method = httpPrack;
        return 0;
    }
    if (! http_ncasecmp(name, "CANCEL"))
    {
        *method = httpCancel;
        return 0;
    }
    if (! http_ncasecmp(name, "UPDATE"))
    {
        *method = httpUpdate;
        return 0;
    }
    if (! http_ncasecmp(name, "INFO"))
    {
        *method = httpInfo;
        return 0;
    }
    if (! http_ncasecmp(name, "SUBSCRIBE"))
    {
        *method = httpSubscribe;
        return 0;
    }
    if (! http_ncasecmp(name, "NOTIFY"))
    {
        *method = httpNotify;
        return 0;
    }
    if (! http_ncasecmp(name, "REFER"))
    {
        *method = httpRefer;
        return 0;
    }
    if (! http_ncasecmp(name, "MESSAGE"))
    {
        *method = httpMessage;
        return 0;
    }
    if (! http_ncasecmp(name, "REGISTER"))
    {
        *method = httpRegister;
        return 0;
    }
    if (! http_ncasecmp(name, "BYE"))
    {
        *method = httpBye;
        return 0;
    }
#endif
    *method = httpUnsupported;
    return -1;
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
