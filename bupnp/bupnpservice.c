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
#include "bupnp.h"

int upnp_add_device(
                    upnp_server_t	*server,
					const uuid_t     udn,
					const char      *description_url,
					const char      *device_name,
					const uint32_t   device_version,
					const uint32_t   advertising_rate
                    )
{
    upnp_device_t *device;
	upnp_device_t *pd;

	if (! server)
	{
		return -1;
	}

	if (! server->root_device)
	{
		butil_log(4, "Adding root device %s\n", device_name);
	}
	else
	{
		butil_log(5, "Adding embedded device %s\n", device_name);
	}

	device = (upnp_device_t *)malloc(sizeof(upnp_device_t));
	if (! device)
	{
		return -1;
	}
	strncpy(device->desc_url, description_url, sizeof(device->desc_url));
	strncpy(device->upnp_device_type, device_name, sizeof(device->upnp_device_type));
	device->upnp_device_version = device_version;

	device->adv_rate = advertising_rate;
	device->next_adv = 0;
	uuid_copy(device->udn, udn);
	device->next = NULL;

	if (! server->root_device)
	{
		device->root = NULL;
		server->root_device = device;
	}
	else
	{
		device->root = server->root_device;
		for (pd = server->root_device; pd->next;)
		{
			pd = pd->next;
		}
		pd->next = device;
	}

    return 0;
}

int upnp_add_service(
                    upnp_server_t      *server,
                    upnp_device_t      *device,
                    upnp_callback_t     callback,
                    const char         *service_type,
                    const uint32_t      service_version,
                    const char         *service_url,
                    const char         *control_url,
                    const char         *event_url
                    )
{
    upnp_service_t *service;
	upnp_service_t *ps;

	if (! server || ! device)
	{
		return -1;
	}
    service = (upnp_service_t *)malloc(sizeof(upnp_service_t));
    if (! service)
    {
        return -1;
    }
    memset(service, 0, sizeof(upnp_service_t));

	strncpy(service->upnp_service_type, service_type, sizeof(service->upnp_service_type));
	service->upnp_service_version = service_version;

    service->callback = callback;

    if (service_url != NULL)
    {
        strncpy(service->scpd_url, service_url, UPNP_MAX_URL);
        service->scpd_url[UPNP_MAX_URL - 1] = '\0';
    }

    if (control_url != NULL)
    {
        strncpy(service->ctrl_url, control_url, UPNP_MAX_URL);
        service->ctrl_url[UPNP_MAX_URL - 1] = '\0';
    }

    if (event_url != NULL)
    {
        strncpy(service->evnt_url, event_url, UPNP_MAX_URL);
        service->evnt_url[UPNP_MAX_URL - 1] = '\0';
    }

	service->device = device;
	service->next = NULL;

	// append service on device's service list
	//
	if (! device->services)
	{
		device->services = service;
	}
	else
	{
		for (ps = device->services; ps->next;)
		{
			ps = ps->next;
		}
		ps->next = service;
	}
    return 0;
}


