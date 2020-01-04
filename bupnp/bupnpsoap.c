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

int upnp_set_state_var(upnp_var_t *var, const char *value)
{
	if (! var || ! value)
	{
		return -1;
	}

	if (var->alloced)
	{
		free(var->val.sval);
		var->val.sval = NULL;
		var->alloced = false;
	}

	switch (var->type)
	{
	default:
	case upnp_dt_unknown:
	case upnp_dt_function:
		butil_log(2, "Unimplemented type\n");
		break;
	case upnp_dt_ui1:
	case upnp_dt_ui2:
	case upnp_dt_ui4:
		var->val.uval = strtoul(value, NULL, 0);
		break;
	case upnp_dt_i1:
	case upnp_dt_12:
	case upnp_dt_i4:
	case upnp_dt_int:
		var->val.ival = strtol(value, NULL, 0);
		break;
	case upnp_dt_r4:
	case upnp_dt_r8:
	case upnp_dt_number:
	case upnp_dt_fixed14:
		butil_log(2, "Unimplemented type\n");
		break;
	case upnp_dt_float:
		butil_log(2, "Unimplemented type\n");
		break;
	case upnp_dt_char:
		var->val.ival = value[0];
		break;
	case upnp_dt_string:
	case upnp_dt_uri:
	case upnp_dt_uuid:
	case upnp_dt_base64:
	case upnp_dt_data:
	case upnp_dt_datetime:
	case upnp_dt_datetimetz:
	case upnp_dt_time:
	case upnp_dt_timetz:
	case upnp_dt_hex:
		var->val.sval = (char*)malloc(strlen(value) + 1);
		if (! var->val.sval)
		{
			return -1;
		}
		var->alloced = true;
		strcpy(var->val.sval, value);
		break;
	case upnp_dt_bool:
		var->val.ival = 0;
		if (! strcasecmp(value, "true"))
		{
			var->val.ival = 1;
		}
		if (! strcasecmp(value, "yes"))
		{
			var->val.ival = 1;
		}
		break;
	}
	return 0;
}

int upnp_get_state_var_as_int(upnp_var_t *var, int *value)
{
	if (! var || ! value)
	{
		return -1;
	}

	*value = 0;

	switch (var->type)
	{
	default:
	case upnp_dt_unknown:
	case upnp_dt_function:
		return -1;
	case upnp_dt_ui1:
	case upnp_dt_ui2:
	case upnp_dt_ui4:
		*value = (int)var->val.uval;
		break;
	case upnp_dt_i1:
	case upnp_dt_12:
	case upnp_dt_i4:
	case upnp_dt_int:
		*value = (int)var->val.ival;
		break;
	case upnp_dt_r4:
	case upnp_dt_r8:
	case upnp_dt_number:
	case upnp_dt_fixed14:
		butil_log(2, "Unimplemented type\n");
		break;
	case upnp_dt_float:
		butil_log(2, "Unimplemented type\n");
		break;
	case upnp_dt_char:
		*value = (int)(char)var->val.ival;
		break;
	case upnp_dt_string:
	case upnp_dt_uri:
	case upnp_dt_uuid:
	case upnp_dt_base64:
	case upnp_dt_data:
	case upnp_dt_datetime:
	case upnp_dt_datetimetz:
	case upnp_dt_time:
	case upnp_dt_timetz:
	case upnp_dt_hex:
		// if sval is ONLY numbers, no problem
		if (! var->val.sval)
		{
			return -1;
		}
		*value = (int)strtoul(var->val.sval, NULL, 0);
		break;
	case upnp_dt_bool:
		*value = var->val.ival != 0;
		break;
	}
	return 0;
}

int upnp_get_arg_value(
						upnp_service_t *service,
						const char *action_name,
						const char *arg_name,
						upnp_var_t **pvar
					  )
{
	upnp_action_t *action;
	upnp_arglist_t *arg;

	if (! service || ! action_name || ! arg_name || ! pvar)
	{
		return -1;
	}
	*pvar = NULL;

	action = upnp_action_from_name(service, action_name);
	if (! action)
	{
		return -1;
	}

	arg = upnp_arg_from_name(action, arg_name);
	if (! arg)
	{
		return -1;
	}

	if (! arg->isset)
	{
		butil_log(4, "Argument %s not set in call %s\n",
					arg_name, action_name);
		return -1;
	}

	*pvar = arg->var;
	return 0;
}

int upnp_get_arg_value_as_int(
						upnp_service_t *service,
						const char *action_name,
						const char *arg_name,
						int *pval
					  )
{
	upnp_var_t *var;
	int result;

	if (! pval)
	{
		return -1;
	}
	*pval = 0;

	result = upnp_get_arg_value(service, action_name, arg_name, &var);
	if (result)
	{
		return result;
	}

	result = upnp_get_state_var_as_int(var, pval);
	return result;
}

int upnp_dispatch_soap(upnp_server_t *server, upnp_service_t *service, const char *action_name)
{
	bxml_parser_t body_parser;
	bxml_parser_t *pxpb;
	bxml_parser_t action_parser;
	bxml_parser_t *pxpa;
	const char *ptag;

	int result;

	if (! server || ! service || ! action_name)
	{
		return -1;
	}
	butil_log(5, "Action %s on service %s\n", action_name, service->usn);

	pxpb = bxml_parser_create(&body_parser, (char*)server->soap.data);
	if (! pxpb)
	{
		butil_log(1, "Can't make soap xml parser\n");
		return -2;
	}

	// locate the element "Envelope.Body.<action>" where action is the SOAP action
	//
	result = bxml_find_element(
								pxpb,
								"Envelope.Body", '.',
								0,
								&ptag
   							);
	if (! result)
	{
		result = bxml_find_next_element(pxpb, action_name, '.', 0, &ptag);
	}

	if (! result)
	{
		upnp_action_t *action;
		upnp_arglist_t *arg;
		char argval[256];

		// lookup the action in the service
		//
		action = upnp_action_from_name(service, action_name);

		if (! action)
		{
			butil_log(3, "No action %s in service %s\n", action_name, service->usn);
		}
		else
		{
			// xml sub-parser on the action parameters
			//
		    pxpa = bxml_parser_create(&action_parser, ptag);

			if (! result)
			{
				for (arg = action->args; arg; arg = arg->next)
				{
					if (arg->inOUT)
					{
						// dont expect out-args in the in list
						//
						continue;
					}

					arg->isset = false;

					// for each action argument, extract arg from action element
					//
					result = bxml_find_and_copy_element(
														pxpa,
														arg->name, '.',
														0,
														argval,
														sizeof(argval),
														false,
														false
														);

					if (! result)
					{
						butil_log(3, "Arg %s value=%s=\n", arg->name, argval);

						arg->isset = true;

						if (arg->var)
						{
							result = upnp_set_state_var(arg->var, argval);
						}
					}
					else
					{
						butil_log(2, "Argument %s missing in action %s\n", arg->name, action_name);
					}
				}
			}

			bxml_parser_destroy(pxpa);
		}
	}

	bxml_parser_destroy(pxpb);

	// if the service has a callback, do that
	//
	result = service->callback(server, service, action_name);

	return result;
}

int upnp_handle_control_url(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     	)
{
    upnp_server_t *server;
	upnp_device_t *device;
	upnp_service_t *service;
    char *header;
    char *value;
	char  usn[UPNP_MAX_URL];
	char *pusn;
	char *pact;
	char *psrc;
	int len;
    int result;

    server = (upnp_server_t *)resource->priv;
    if (! server)
    {
        return -1;
    }

    result = 0;

    butil_log(5, "ControlCB %d %s to %s\n", cbtype, http_method_name(client->method), client->path);

    switch (cbtype)
    {
    case httpRequest:

        client->keepalive = false;

        if (client->method == httpPost)
        {
            butil_log(4, "POST %s %s\n", client->path, server->soap_header);
			iostream_reset_ring(&server->soap);
            return 0;
        }
        else
        {
            butil_log(1, "Can't do method \"%s\" on UPnP\n",
                   http_method_name(client->method));
            return -1;
        }
        break;

    case httpRequestHeader:

        header = (char*)*data;
        value = header;
        if (! header)
        {
            break;
        }

        while (*value != ' ' && *value != '\t' && *value)
        {
            value++;
        }

        while (*value == ' ' || *value == '\t')
        {
            value++;
        }

        if (! http_ncasecmp(header, "soapaction:"))
        {
            // save action header,
			strncpy(server->soap_header, value, sizeof(server->soap_header));
            break;
        }
        break;

    case httpDownloadData:

		// copy data from http client to our ring buffer
		//
		{
			size_t ndata = *count;
			size_t room  = server->soap.size - server->soap.count - 1;
			uint8_t *input = (data != NULL) ? *data : NULL;

			if (input && ndata)
			{
				if (ndata > room)
				{
					butil_log(2, "Truncating soap\n");
					ndata = room;
				}
				memcpy(server->soap.data + server->soap.head, input, ndata);
				server->soap.head += ndata;
				server->soap.count += ndata;
				server->soap.data[server->soap.head] = '\0';
			}
			butil_log(6, "Got %u soap, for %u tot\n", ndata, server->soap.count);
		}
        break;

    case httpDownloadDone:

		butil_log(3, "ControlCB: SOAP %s %u bytes\n", server->soap_header, server->soap.count);

		// figure out which service this control is intended for by comparing usn's
		//
		pusn = server->soap_header;

		// incoming usn is between " and #, so extract it
		if (*pusn == '\"')
		{
			pusn++;
		}

		for (psrc = pusn; *psrc && *psrc != '#';)
		{
			psrc++;
		}

		if (*psrc == '#')
		{
			*psrc++ = '\0';
		}
		else
		{
			butil_log(3, "No action for SOAP\n");
			break;
		}

		// action is between # and " so extract that
		//
		pact = psrc;
		while (*psrc && *psrc != '\"')
		{
			psrc++;
		}
		*psrc = '\0';

		service = NULL;
		for (device = server->root_device; device; device = device->next)
		{
			for (service = device->services; service; service = service->next)
			{
                snprintf(usn, sizeof(usn), "urn:schemas-upnp-org:service:%s:", service->upnp_service_type);

				// compare to search target
				//
				butil_log(3, "Compare =%s= to our =%s=\n", pusn, usn);

				if (! http_ncasecmp(pusn, usn))
				{
					// partially matched a service, so check our version is >= the req ver
					//
					uint32_t theirver = strtoul(pusn + strlen(usn), NULL, 0);

					if (service->upnp_service_version >= theirver)
					{
						butil_log(3, "Control target matches %s\n", usn);
						break;
					}
				}
			}
		}
		if (! service)
		{
			butil_log(4, "No service \"%s\" on any device\n", usn);
			break;
		}
		server->soap_header[0] = '\0';

		// extract parameters and call the service's callback with the action
		// if there's a reply, set the content length and upload it next
		//
		result = upnp_dispatch_soap(server, service, pact);

		break;

    case httpUploadData:
        *count = 0;
        break;

    case httpComplete:
        break;

    default:
        return -1;
    }
    return result;
}

int upnp_handle_event_url(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    upnp_server_t *server;
    char *header;
    char *value;
    int result;

    server = (upnp_server_t *)resource->priv;
    if (! server)
    {
        return -1;
    }

    result = 0;

    butil_log(5, "ControlCB %d %s to %s\n", cbtype, http_method_name(client->method), client->path);

    switch (cbtype)
    {
    case httpRequest:

        client->keepalive = false;

        if (client->method == s_method_SUBSCRIBE)
        {
            butil_log(4, "SUBSCRIBE %s\n", client->path);
            return 0;
        }
        else if (client->method == s_method_UNSUBSCRIBE)
        {
            butil_log(4, "UNSUBSCRIBE %s\n", client->path);
            return 0;
        }
        else
        {
            butil_log(1, "Can't do method \"%s\" on UPnP\n",
                   http_method_name(client->method));
            return -1;
        }
        break;

    case httpRequestHeader:

        header = (char*)*data;
        value = header;
        if (! header)
        {
            break;
        }

        while (*value != ' ' && *value != '\t' && *value)
        {
            value++;
        }

        while (*value == ' ' || *value == '\t')
        {
            value++;
        }

        if (! http_ncasecmp(header, "soapaction:"))
        {
            // decode SOAP header
            break;
        }
        break;

    case httpDownloadData:
        http_log(4, "Download %s", *data);
        break;

    case httpDownloadDone:
        break;

    case httpUploadData:
        *count = 0;
        break;

    case httpComplete:
        break;

    default:
        return -1;
    }
    return result;
}


