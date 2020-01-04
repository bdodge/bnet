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

typedef enum
{
    upnp_dt_unknown,
    upnp_dt_function,
    upnp_dt_ui1,
    upnp_dt_ui2,
    upnp_dt_ui4,
    upnp_dt_i1,
    upnp_dt_12,
    upnp_dt_i4,
    upnp_dt_int,
    upnp_dt_r4,
    upnp_dt_r8,
    upnp_dt_number,
    upnp_dt_fixed14,
    upnp_dt_float,
    upnp_dt_char,
    upnp_dt_string,
    upnp_dt_data,
    upnp_dt_datetime,
    upnp_dt_datetimetz,
    upnp_dt_time,
    upnp_dt_timetz,
    upnp_dt_bool,
    upnp_dt_base64,
    upnp_dt_hex,
    upnp_dt_uri,
    upnp_dt_uuid
}
upnp_vartype_t;

typedef struct upnp_var
{
    char               *name;
    upnp_vartype_t      type;
    bool                alloced;
    union
    {
        char    *sval;
        uint32_t uval;
        int32_t  ival;
        double   fval;
    }
                        val;
    struct upnp_var    *next;
}
upnp_var_t;

typedef struct upnp_arglist
{
    char               *name;
    bool                inOUT;
    bool                isset;
    upnp_var_t         *var;
    struct upnp_arglist *next;
}
upnp_arglist_t;

typedef struct upnp_action
{
    char               *name;
    upnp_arglist_t     *args;
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
    char                udn     [UPNP_MAX_URL];
    char                loc     [UPNP_MAX_URL];
    char                url     [UPNP_MAX_URL];

    char                sid     [UPNP_MAX_URL];

    time_t              lastping;
    time_t              rate;

    unsigned long       seq;

    upnp_callback_t     callback;
    struct upnp_subscription *next;
}
upnp_subscription_t;

int upnp_get_arg_value_as_int(
                        upnp_service_t *service,
                        const char *action_name,
                        const char *arg_name,
                        int *pval
                      );
upnp_var_t *upnp_state_var_from_name(upnp_service_t *service, const char *var_name);
upnp_arglist_t *upnp_arg_from_name(upnp_action_t *action, const char *arg_name);
upnp_action_t *upnp_action_from_name(upnp_service_t *service, const char *action_name);

#endif
