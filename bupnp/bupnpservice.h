/*
 * Copyright 2019 Brian Dodge
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    upnp://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef BUPNPSERVICE_H
#define BUPNPSERVICE_H 1

#include "bupnpuuid.h"

// major service state
//
typedef enum
{
    upnpServiceInit,
    upnpServiceOnline,
    upnpServiceOffline
}
upnp_service_state_t;

struct upnp_server;
struct upnp_device;
struct upnp_service;

typedef int (*upnp_callback_t)(struct upnp_server *server, struct upnp_service *service, const char *action);

typedef struct upnp_arglist
{
    char               *name;
    upnp_val_t          val;
    upnp_var_t         *var;
    bool                inOUT;
    bool                isset;
    bool                istype;
    struct upnp_arglist *next;
}
upnp_arglist_t;

typedef struct upnp_action
{
    char               *name;
    upnp_arglist_t     *args;
    upnp_arglist_t     *results;
    struct upnp_action *next;
}
upnp_action_t;

typedef struct upnp_device
{
    uuid_t              udn;
    char                desc_url[UPNP_MAX_URL];
    char                upnp_device_type[UPNP_MAX_URL];
    uint32_t            upnp_device_version;
    time_t              next_adv;
    time_t              adv_rate;

    struct upnp_device  *root;
    struct upnp_device  *next;
    struct upnp_service *services;
}
upnp_device_t;

typedef struct upnp_service
{
    uuid_t              usn;
    char                scpd_url[UPNP_MAX_URL];
    char                event_url[UPNP_MAX_URL];
    char                upnp_service_type[UPNP_MAX_URL];
    uint32_t            upnp_service_version;
    upnp_callback_t     callback;

    upnp_action_t       *actions;
    upnp_var_t          *state_vars;

    struct upnp_device  *device;
    struct upnp_service *next;
}
upnp_service_t;

typedef struct upnp_subscription
{
    upnp_service_t     *service;
    char                loc[UPNP_MAX_URL];
    char                sid[UPNP_MAX_URL];
    time_t              expiry;
    uint32_t            seq;
    bool                dirty;
    upnp_callback_t     callback;
    struct upnp_subscription *next;
}
upnp_subscription_t;

upnp_var_t      *upnp_state_var_from_name(
                    upnp_service_t      *service,
                    const char          *var_name
                );

upnp_arglist_t  *upnp_arg_from_name(
                    upnp_action_t       *action,
                    const char          *arg_name
                );

upnp_action_t   *upnp_action_from_name(
                    upnp_service_t      *service,
                    const char          *action_name
                );

int             upnp_mark_var_dirty(
                    upnp_service_t      *service,
                    const char          *var_name,
                    bool                isdirty
                );

bool            upnp_any_var_dirty(upnp_service_t *service);

int             upnp_add_device(
                     struct upnp_server *server,
                     const uuid_t       udn,
                     const char        *description_url,
                     const char        *device_type,
                     const uint32_t     device_version,
                     const uint32_t     advertising_rate
                );

int             upnp_add_service(
                    struct upnp_server *server,
                    upnp_device_t      *device,
                    upnp_callback_t     callback,
                    const char         *service_type,
                    const uint32_t      service_version,
                    const char         *scpd_url,
                    const char         *scpd_content,
                    const char         *control_url,
                    const char         *event_url
                );


#endif
