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
#ifndef HTTPUTIL_H
#define HTTPUTIL_H 1

#define HTTP_LOG_LEVEL(n) ((n) & 0xFFFF)
#define HTTP_LOG_TAG(n)   (((uint32_t)(n)) >> 16)
#define LOGTAG(t, n)      ((((uint32_t)(t)) << 16) | ((uint32_t)(n) & 0xFFFF))

#define http_log butil_log
#define http_set_log_level butil_set_log_level

typedef enum
{
    httpMethodRequest,
    httpMethodHeader,
}
http_method_callback_type_t;

typedef int (*http_method_callback_t)(
                            struct http_client *client,
                            http_method_callback_type_t type,
                            const http_method_t method,
                            const char *data,
                            void *priv
                            );

typedef struct tag_user_method
{
    char name[HTTP_MAX_METHOD_NAME];
    http_method_callback_t callback;
    void *priv;
}
http_user_method_t;

int http_join_path          (
                            char *path,
                            size_t room,
                            const char *root,
                            const char *base,
                            const char *file
                            );

int http_ncasecmp           (
                            const char *haystack,
                            const char *needle
                            );
const butil_url_scheme_t http_scheme_base(
                            const butil_url_scheme_t scheme
                            );
const char *http_scheme_base_name(
                            butil_url_scheme_t method
                            );

const char *http_method_name(
                            http_method_t method
                            );
int http_method_from_name   (
                            const char *name,
                            http_method_t *method
                            );
int http_register_method    (
                            const char *name,
                            http_method_callback_t callback,
                            void *priv,
                            http_method_t *methodused
                            );
const http_user_method_t *http_get_user_method(
                            http_method_t method
                            );
int http_make_user_method_callback(
                            struct http_client *client,
                            http_method_callback_type_t type,
                            http_method_t method,
                            const char *data
                            );

int http_auth_string_to_type(
                            const char *auth_str,
                            http_auth_type_t *auth_type
                            );
const char *http_auth_type_to_string(
                            http_auth_type_t auth_type
                            );

#endif

