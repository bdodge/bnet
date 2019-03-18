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
#ifndef HTTPRES_H
#define HTTPRES_H 1

typedef enum
{
    httpRequest,
    httpDownloadData,
    httpUploadData,
    httpComplete
}
http_callback_type_t;

typedef enum {
    httpFileResource,
    httpFunctionResource,
    httpCannedResource,
    httpDavResource
}
http_resource_type_t;

struct http_resource;
struct http_client;

/// http resource function
///
/// for each http method, the resource function will be called for a matching url
/// with httpRequest and httpComplete types.  For GET or PUT requests the function will
/// also be called with httpDownloadData and httpUploadData (in that order for GET
/// and the reverse order for PUT).  WebSockets will call with Download or Upload in
/// any order.
///
/// for httpDownloadData, data will point to a buffer containing the data to absorb
/// and count will point to the byte count of that buffer.  the resource function wil
/// set *count to the number of bytes taken from that buffer.
///
/// for httpUploadData, if *data is NULL, the caller expects data to be set by
/// the resource function to an internal buffer.  *count should be set to the largest
/// number of bytes the caller can read from the function provided buffer and the
/// function will set *count to that, or the number of bytes available, whichever is smaller.
//  The caller must absorb *count available bytes from that buffer. If data is NULL,
/// *count is set to the number of bytes available to upload from the function but
/// the function keeps those bytes. if *data is non-NULL, *count should be set to
/// the space in the buffer pointed to by data when calling and the resource function
/// should set *count to the number of bytes copied into *data
///
/// @param client    the http client context
/// @param resource  the resource descriptor
/// @param cbtype    the reason the function is being called
/// @param data      for download (to server) a pointer to the source of data
///                  for upload (form server) a pointer to a pointer to place data
///                  or NULL in which case the function should set the pointer to its data
/// @param count     [in/out] in:the length of data in data supplied or length of buffer
///                  supplied for output data out: the bytes taken from data or copied to data
///                  or presented in data
/// @returns status: 0 ok, non-0 error
///
typedef int (*http_resource_function_t)(
                            struct http_client   *client,
                            struct http_resource *resource,
                            http_callback_type_t  cbtype,
                            uint8_t             **data,
                            size_t               *count
                            );

typedef struct
{
    const char *user;
    const char *pass;
    http_auth_type_t type;
}
http_credentials_t;

typedef struct http_resource
{
    struct http_resource       *next;
    butil_url_scheme_t          scheme;
    const char                 *urlbase;
    http_resource_type_t        type;
    #if HTTP_SUPPORT_AUTH
    http_credentials_t          credentials;
    #endif
    http_resource_function_t    callback;
    void                       *priv;

    union
    {
        struct
        {
            const char         *root;
        }
        file_data;

        struct
        {
            mime_content_type_t content_type;
            const uint8_t      *content;
            size_t              count;
        }
        canned_data;
    }
    resource;
}
http_resource_t;

http_resource_t *http_resource_create(void);
void http_resource_free(http_resource_t *resource);

int http_add_file_resource(
                        http_resource_t   **resources,
                        const butil_url_scheme_t scheme,
                        const char         *urlbase,
                        const char         *root,
                        http_credentials_t *credentials
                        );

int http_add_func_resource(
                        http_resource_t   **resources,
                        const butil_url_scheme_t scheme,
                        const char         *urlbase,
                        http_credentials_t *credentials,
                        http_resource_function_t func,
                        void               *priv
                        );

int http_add_canned_resource(
                        http_resource_t   **resources,
                        const butil_url_scheme_t scheme,
                        const char         *urlbase,
                        http_credentials_t *credentials,
                        mime_content_type_t content_type,
                        const uint8_t      *content,
                        const size_t        content_length
                        );
#if HTTP_SUPPORT_WEBDAV
int http_add_dav_resource(
                        http_resource_t   **resources,
                        const char         *urlbase,
                        const char         *root,
                        http_credentials_t *credentials
                        );
#endif
#if HTTP_SUPPORT_SIP
int http_add_sip_resource(
                        http_resource_t   **resources,
                        const char         *urlbase,
                        const char         *root,
                        http_credentials_t *credentials
                        );
#endif
http_resource_t *http_match_resource(
                        http_resource_t    *resources,
                        const butil_url_scheme_t scheme,
                        const char         *urlpath,
                        http_resource_type_t type
                        );

http_resource_t *http_find_resource(
                        http_resource_t *resources,
                        const butil_url_scheme_t scheme,
                        const char      *path,
                        http_method_t    method
                        );

int http_check_credentials(
                        http_auth_type_t    auth_type,
                        const char         *supplied_creds,
                        http_credentials_t *resource_creds
                        );

#endif
