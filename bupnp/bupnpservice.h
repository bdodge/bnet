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

struct upnp_service;

typedef int (*upnp_callback_t)(void *priv);

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
    char                upnp_service_type[UPNP_MAX_URL];
    uint32_t            upnp_service_version;
    char                scpd_url    [UPNP_MAX_URL];
    char                ctrl_url    [UPNP_MAX_URL];
    char                evnt_url    [UPNP_MAX_URL];

    upnp_callback_t     callback;
//   PUPnPACTION     m_actions;
//   PSOAPVAR        m_vars;

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

#endif
