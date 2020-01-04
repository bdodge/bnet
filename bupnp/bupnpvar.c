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

upnp_vartype_t upnp_type_string_to_type(const char *typestr)
{
    if (! strcmp(typestr, "unknown"))
	{
		return upnp_dt_unknown;
	}
    if (! strcmp(typestr, "function"))
	{
		return upnp_dt_function;
	}
    if (! strcmp(typestr, "ui1"))
	{
		return upnp_dt_ui1;
	}
    if (! strcmp(typestr, "ui2"))
	{
		return upnp_dt_ui2;
	}
    if (! strcmp(typestr, "ui4"))
	{
		return upnp_dt_ui4;
	}
    if (! strcmp(typestr, "i1"))
	{
		return upnp_dt_i1;
	}
    if (! strcmp(typestr, "12"))
	{
		return upnp_dt_12;
	}
    if (! strcmp(typestr, "i4"))
	{
		return upnp_dt_i4;
	}
    if (! strcmp(typestr, "int"))
	{
		return upnp_dt_int;
	}
    if (! strcmp(typestr, "r4"))
	{
		return upnp_dt_r4;
	}
    if (! strcmp(typestr, "r8"))
	{
		return upnp_dt_r8;
	}
    if (! strcmp(typestr, "number"))
	{
		return upnp_dt_number;
	}
    if (! strcmp(typestr, "fixed14"))
	{
		return upnp_dt_fixed14;
	}
    if (! strcmp(typestr, "float"))
	{
		return upnp_dt_float;
	}
    if (! strcmp(typestr, "char"))
	{
		return upnp_dt_char;
	}
    if (! strcmp(typestr, "string"))
	{
		return upnp_dt_string;
	}
    if (! strcmp(typestr, "data"))
	{
		return upnp_dt_data;
	}
    if (! strcmp(typestr, "datetime"))
	{
		return upnp_dt_datetime;
	}
    if (! strcmp(typestr, "datetimetz"))
	{
		return upnp_dt_datetimetz;
	}
    if (! strcmp(typestr, "time"))
	{
		return upnp_dt_time;
	}
    if (! strcmp(typestr, "timetz"))
	{
		return upnp_dt_timetz;
	}
    if (! strcmp(typestr, "bool"))
	{
		return upnp_dt_bool;
	}
    if (! strcmp(typestr, "base64"))
	{
		return upnp_dt_base64;
	}
    if (! strcmp(typestr, "hex"))
	{
		return upnp_dt_hex;
	}
    if (! strcmp(typestr, "uri"))
	{
		return upnp_dt_uri;
	}
    if (! strcmp(typestr, "uuid"))
	{
		return upnp_dt_uuid;
	}

	return upnp_dt_int;
}

int upnp_set_state_var(upnp_var_t *var, const char *value)
{
	if (! var || ! value)
	{
		return -1;
	}

	if (var->val.alloclen > 0)
	{
		free(var->val.value.sval);
		var->val.value.sval = NULL;
		var->val.alloclen = 0;
	}

	switch (var->val.type)
	{
	default:
	case upnp_dt_unknown:
	case upnp_dt_function:
		butil_log(2, "Unimplemented type\n");
		break;
	case upnp_dt_ui1:
	case upnp_dt_ui2:
	case upnp_dt_ui4:
		var->val.value.uval = strtoul(value, NULL, 0);
		break;
	case upnp_dt_i1:
	case upnp_dt_12:
	case upnp_dt_i4:
	case upnp_dt_int:
		var->val.value.ival = strtol(value, NULL, 0);
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
		var->val.value.ival = value[0];
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
		var->val.alloclen = strlen(value) + 1;
		var->val.value.sval = (char*)malloc(var->val.alloclen);
		if (! var->val.value.sval)
		{
			var->val.alloclen = 0;
			return -1;
		}
		strcpy(var->val.value.sval, value);
		break;
	case upnp_dt_bool:
		var->val.value.ival = 0;
		if (! strcasecmp(value, "true"))
		{
			var->val.value.ival = 1;
		}
		if (! strcasecmp(value, "yes"))
		{
			var->val.value.ival = 1;
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

	switch (var->val.type)
	{
	default:
	case upnp_dt_unknown:
	case upnp_dt_function:
		return -1;
	case upnp_dt_ui1:
	case upnp_dt_ui2:
	case upnp_dt_ui4:
		*value = (int)var->val.value.uval;
		break;
	case upnp_dt_i1:
	case upnp_dt_12:
	case upnp_dt_i4:
	case upnp_dt_int:
		*value = (int)var->val.value.ival;
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
		*value = (int)(char)var->val.value.ival;
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
		if (! var->val.value.sval)
		{
			return -1;
		}
		*value = (int)strtoul(var->val.value.sval, NULL, 0);
		break;
	case upnp_dt_bool:
		*value = var->val.value.ival != 0;
		break;
	}
	return 0;
}

