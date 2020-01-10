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
#include "bxml.h"

static bool upnp_dir_is_out(const char *direction)
{
    if (! strcmp(direction, "out"))
    {
        return true;
    }
    return false;
}

static int upnp_add_var_to_service(upnp_service_t *service, const char *name, const char *type, bool eventable)
{
    upnp_var_t *var;

    if (! service || ! name || ! type)
    {
        return -1;
    }

    var = (upnp_var_t *)malloc(sizeof(upnp_var_t));
    if (! var)
    {
        return -2;
    }

    var->name = (char *)malloc(strlen(name) + 1);
    if (! var->name)
    {
        free(var);
        return -3;
    }

    strcpy(var->name, name);
    var->val.alloclen = 0;
    var->val.slen = 0;
    var->val.value.uval = 0;
    var->val.type = upnp_type_string_to_type(type);

    if (eventable)
    {
        // var is not, but could be, dirty
        var->dirty = vdsNone;
    }
    else
    {
        // var can't be made dirty ever
        var->dirty = vdsNeverDirty;
    }

    var->next = service->state_vars;
    service->state_vars = var;

    return 0;
}

upnp_var_t *upnp_state_var_from_name(upnp_service_t *service, const char *var_name)
{
    upnp_var_t *var;

    for (var = service->state_vars; var; var = var->next)
    {
        if (! strcmp(var->name, var_name))
        {
            return var;
        }
    }

    return NULL;
}

upnp_arglist_t *upnp_arg_from_name(upnp_action_t *action, const char *arg_name)
{
    upnp_arglist_t *args;

    for (args = action->args; args; args = args->next)
    {
        if (! strcmp(args->name, arg_name))
        {
            return args;
        }
    }

    return NULL;
}

upnp_action_t *upnp_action_from_name(upnp_service_t *service, const char *action_name)
{
    upnp_action_t *action;

    for (action = service->actions; action; action = action->next)
    {
        if (! strcmp(action->name, action_name))
        {
            return action;
        }
    }

    return NULL;
}

int upnp_mark_var_dirty(
                    upnp_service_t      *service,
                    const char          *var_name,
                    bool                isdirty
                )
{
    upnp_var_t *var;
	bool istype;

	if (! service)
	{
		return -1;
	}

	if (! var_name)
	{
		var = service->state_vars;
	}
	else
	{
		var = upnp_state_var_from_name(service, var_name);
	}

	if (! var)
	{
		return -1;
	}

	do
	{
		istype = !strncmp(var->name, "A_ARG_TYPE_", 11);
		if (! istype)
		{
			upnp_set_var_dirty(var, isdirty);
		}

		if (! var_name)
		{
			var = var->next;
		}
		else
		{
			var = NULL;
		}
	}
	while (var);

	return 0;
}


bool upnp_any_var_dirty(upnp_service_t *service)
{
    upnp_var_t *var;
	bool istype;

	if (! service)
	{
		return false;
	}

	for (var = service->state_vars; var; var = var->next)
	{
		istype = !strncmp(var->name, "A_ARG_TYPE_", 11);
		if (! istype)
		{
			if (upnp_get_var_dirty(var))
			{
				return true;
			}
		}
	}

	return false;
}

static int upnp_add_arg_to_action(
                                upnp_service_t *service,
                                const char *action_name,
                                const char *arg_name,
                                const char *var_name,
                                const char *direction
                                )
{
    upnp_action_t *action;
    upnp_arglist_t *arg;
    upnp_var_t *var;
    upnp_vartype_t type;

    if (! service || ! action_name || ! arg_name)
    {
        return -1;
    }

    action = upnp_action_from_name(service, action_name);
    if (! action)
    {
        butil_log(1, "No action named %s to add arg %s to\n",
                            action_name, arg_name);
        return -2;
    }

    var = upnp_state_var_from_name(service, var_name);
    if (! var)
    {
        butil_log(1, "No statevar %s for arg %s of action %s\n",
                            var_name, arg_name, action_name);
        return -2;
    }

    arg = (upnp_arglist_t *)malloc(sizeof(upnp_arglist_t));
    if (! arg)
    {
        return -2;
    }

    arg->name = (char *)malloc(strlen(arg_name) + 1);
    if (! arg->name)
    {
        free(var);
        return -3;
    }

    strcpy(arg->name, arg_name);

    arg->var = var;
    arg->inOUT = upnp_dir_is_out(direction);
    arg->isset = false;
    arg->istype = !strncmp(var->name, "A_ARG_TYPE_", 11);
    // inherit type from related statevar
    arg->val.type = var->val.type;
    arg->val.alloclen = 0;
    arg->val.slen = 0;
    arg->val.value.uval = 0;
    arg->next = NULL;

    if (! action->args)
    {
        action->args = arg;
    }
    else
    {
        upnp_arglist_t *pa;

        for (pa = action->args; pa->next;)
        {
            pa = pa->next;
        }

        pa->next = arg;
    }

    return 0;
}

static int upnp_add_action_to_service(upnp_service_t *service, const char *name)
{
    upnp_action_t *action;

    if (! service || ! name)
    {
        return -1;
    }

    action = (upnp_action_t *)malloc(sizeof(upnp_action_t));
    if (! action)
    {
        return -2;
    }

    action->name = (char *)malloc(strlen(name) + 1);
    if (! action->name)
    {
        free(action);
        return -3;
    }

    strcpy(action->name, name);

    action->next = service->actions;
    service->actions = action;

    return 0;
}

#if UPNP_HANDLE_CANNED_URLS
int upnp_handle_scpd_url(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    const char *payload = (const char*)resource->priv;
    int paylen = strlen(payload);
    size_t offset = (size_t)(intptr_t)client->ctxpriv;
    size_t moved;
    size_t have;

    switch (cbtype)
    {
    case httpRequestHeader:
        break;

    case httpRequest:
        client->ctxpriv = (void*)(intptr_t)0;
        client->keepalive = false;

        switch (client->method)
        {
        case httpGet:
            client->out_content_length = paylen;
            client->out_content_type   = butil_mime_xml;
            break;

        default:
            BERROR("Bad method for scpd data");
            return -1;
        }
        break;

    case httpDownloadData:
        // pretend like we took it all
        return 0;

    case httpDownloadDone:
        return 0;

    case httpUploadData:
        moved = *count;
        have  = paylen - offset;

        if (have < moved)
        {
            moved = have;
        }
        butil_log(5, "move %u of %u at %u\n", moved, have, offset);
        if (moved > 0)
        {
            if (*data)
            {
                memcpy(*data, (uint8_t*)payload + offset, moved);
            }
            else
            {
                *data = (uint8_t*)payload + offset;
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
#endif

static int upnp_parse_scpd(upnp_server_t *server, upnp_service_t *service, const char *scpd)
{
    bxml_parser_t xml_parser;
    bxml_parser_t inner_xml_parser;
    bxml_parser_t arg_xml_parser;
    bxml_parser_t *pxp;
    bxml_parser_t *inner_pxp;
    bxml_parser_t *arg_pxp;
    const char *element;
    const char *ptag;
    const char *pval;
    size_t vallen;
    char vardesc[256];
    char name[64];
    char type[32];
    char relvar[64];
    int index;
    int argdex;
    bool eventable;
    int result;

    if (! server || ! service || ! scpd)
    {
        return -1;
    }
    pxp = bxml_parser_create(&xml_parser, scpd);

    // iterate the state variable list in the scpd
    //
    index = 0;
    do
    {
        char eventval[64];

        // find the index'th stateVariable in the scpd xml
        //
        result = bxml_find_element(
                                    pxp,
                                    "serviceStateTable.stateVariable", '.',
                                    index,
                                    &ptag
                                );
        if (! result)
        {
            eventable = false;

            result = bxml_find_and_copy_attribute(
                                pxp,
                                ptag,
                                "sendEvents",
                                eventval,
                                sizeof(eventval),
                                false, false
                                );

            if (! result)
            {
                if (! http_ncasecmp(eventval, "yes"))
                {
                    eventable = true;
                }
            }
            else
            {
                butil_log(1, "No sendEvent attribute on statevar %s\n", vardesc);
                result = 0;
            }

            result = bxml_parse_value(
                                pxp,
                                ptag,
                                NULL, 0, NULL,
                                &pval,
                                &vallen
                                );
            if (! result)
            {
                result = bxml_copy_element(
                                            vardesc,
                                            sizeof(vardesc),
                                            (char*)pval,
                                            vallen,
                                            false,
                                            false
                                            );
                if (result)
                {
                    butil_log(1, "cant copy value of %s\n", pval);
                }
            }
            else
            {
                butil_log(1, "cant parse value %s\n", ptag);
            }

            inner_pxp = bxml_parser_create(&inner_xml_parser, vardesc);

            result = bxml_find_and_copy_element(inner_pxp, "name", '.', 0, name, sizeof(name), false, false);
            if (! result)
            {
                result = bxml_find_and_copy_element(inner_pxp, "dataType", '.', 0, type, sizeof(type), false, false);

                if (! result)
                {
                    result = upnp_add_var_to_service(service, name, type, eventable);
                    butil_log(3, "Adding sv %3d =%s= =%s= eventable=%s\n", index, name, type, eventable ? "yes" : "no");
                    index++;
                }
            }
            bxml_parser_destroy(inner_pxp);
        }
    }
    while (! result);

    butil_log(3, "Added %d state variables for service\n", index);

    // iterate over action list
    index = 0;
    do
    {
        char action_name[64];

        // find the index'th stateVariable in the scpd xml
        //
        result = bxml_find_element(
                                            pxp,
                                            "actionList.action", '.',
                                            index,
                                            &ptag
                                            );
        if (! result)
        {
            inner_pxp = bxml_parser_create(&inner_xml_parser, ptag);

            result = bxml_find_and_copy_element(
                                            inner_pxp,
                                            "name", '.',
                                            0,
                                            action_name,
                                            sizeof(action_name),
                                            false,
                                            false
                                            );

            if (result)
            {
                bxml_parser_destroy(inner_pxp);
                break;
            }

            result = upnp_add_action_to_service(service, action_name);

            if (result)
            {
                bxml_parser_destroy(inner_pxp);
                break;
            }

            butil_log(3, "Adding action %3d =%s=\n", index, action_name);
            index++;

            argdex = 0;
            do
            {
                result = bxml_find_and_copy_element(
                                            inner_pxp,
                                            "argumentList.argument", '.',
                                            argdex,
                                            vardesc,
                                            sizeof(vardesc),
                                            false,
                                            false
                                            );
                if (! result)
                {
                    arg_pxp = bxml_parser_create(&arg_xml_parser, vardesc);

                    result = bxml_find_and_copy_element(arg_pxp, "name", '.', 0, name, sizeof(name), false, false);
                    if (! result)
                    {
                        result = bxml_find_and_copy_element(arg_pxp, "direction", '.', 0, type, sizeof(type), false, false);
                        if (! result)
                        {
                            result = bxml_find_and_copy_element(arg_pxp, "relatedStateVariable", '.', 0, relvar, sizeof(relvar), false, false);
                            if (! result)
                            {
                                result = upnp_add_arg_to_action(service, action_name, name, relvar, type);

                                butil_log(5, "Adding arg %3d =%s= =%s= rel=%s=\n", argdex, name, type, relvar);
                                argdex++;
                            }
                        }
                    }

                    bxml_parser_destroy(arg_pxp);
                }
            }
            while (! result);

            bxml_parser_destroy(inner_pxp);
            result = 0;
        }
    }
    while (! result);

    butil_log(3, "Added %d actions for service\n", index);

    bxml_parser_destroy(pxp);
    return 0;
}

int upnp_add_device(
                    upnp_server_t   *server,
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
    memset(device, 0, sizeof(upnp_device_t));

    strncpy(device->desc_url, description_url, sizeof(device->desc_url));
    strncpy(device->upnp_device_type, device_name, sizeof(device->upnp_device_type));
    device->upnp_device_version = device_version;

    device->adv_rate = advertising_rate;
    device->next_adv = 0;
    uuid_copy(device->udn, udn);
    device->services = NULL;
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
                    const char         *scpd_url,
                    const char         *scpd_content,
                    const char         *control_url,
                    const char         *event_url
                    )
{
    upnp_service_t *service;
    upnp_service_t *ps;
    int result;

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
    service->state_vars = NULL;
    service->actions = NULL;

    result = 0;

    do // try
    {
        if (scpd_url != NULL)
        {
            strncpy(service->scpd_url, scpd_url, sizeof(service->scpd_url) - 1);
            service->scpd_url[sizeof(service->scpd_url) - 1] = '\0';

            result = upnp_parse_scpd(server, service, scpd_content);
            if (result)
            {
                UPNP_ERROR("can't parse scpd url");
                break;
            }

#if UPNP_HANDLE_CANNED_URLS
            result = upnp_add_func_url(server, scpd_url, upnp_handle_scpd_url, (void*)scpd_content);
#else
            result = upnp_add_text_url(server, scpd_url, butil_mime_xml, scpd_content);
#endif
            if (result)
            {
                UPNP_ERROR("can't add scpd url");
                break;
            }
        }

        if (control_url != NULL)
        {
            result = upnp_add_func_url(server, control_url, upnp_handle_control_url, server);
            if (result)
            {
                UPNP_ERROR("can't add control url");
                break;
            }

        }

        if (event_url != NULL)
        {
            strncpy(service->event_url, event_url, sizeof(service->event_url) - 1);
            service->event_url[sizeof(service->event_url) - 1] = '\0';

            result = upnp_add_func_url(server, event_url, upnp_handle_event_url, server);
            if (result)
            {
                UPNP_ERROR("can't add event url");
                break;
            }
        }
    }
    while (0); // catch

    if (result)
    {
        free(service);
        return result;
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


