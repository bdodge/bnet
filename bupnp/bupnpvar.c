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
    if (! strcmp(typestr, "i2"))
	{
		return upnp_dt_i2;
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

bool upnp_type_is_string_type(const upnp_vartype_t type)
{
	switch (type)
	{
	case upnp_dt_unknown:
	case upnp_dt_function:
	case upnp_dt_ui1:
	case upnp_dt_ui2:
	case upnp_dt_ui4:
	case upnp_dt_i1:
	case upnp_dt_i2:
	case upnp_dt_i4:
	case upnp_dt_int:
	case upnp_dt_r4:
	case upnp_dt_r8:
	case upnp_dt_number:
	case upnp_dt_fixed14:
	case upnp_dt_float:
	case upnp_dt_char:
	case upnp_dt_bool:
		return false;
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
		return true;
	}
	return true;
}

int upnp_set_val_value_from_string(upnp_val_t *val, const char *value)
{
	if (! val || ! value)
	{
		return -1;
	}

	if (val->alloclen > 0)
	{
		free(val->value.sval);
		val->value.sval = NULL;
		val->alloclen = 0;
		val->slen = 0;
	}

	switch (val->type)
	{
	default:
	case upnp_dt_unknown:
	case upnp_dt_function:
		butil_log(2, "Unimplemented type\n");
		break;
	case upnp_dt_ui1:
	case upnp_dt_ui2:
	case upnp_dt_ui4:
		val->value.uval = strtoul(value, NULL, 0);
		break;
	case upnp_dt_i1:
	case upnp_dt_i2:
	case upnp_dt_i4:
	case upnp_dt_int:
		val->value.ival = strtol(value, NULL, 0);
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
		val->value.ival = value[0];
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
		val->slen = strlen(value);
		val->alloclen = val->slen + 1;
		val->value.sval = (char*)malloc(val->alloclen);
		if (! val->value.sval)
		{
			val->alloclen = 0;
			val->slen = 0;
			return -1;
		}
		strcpy(val->value.sval, value);
		break;
	case upnp_dt_bool:
		val->value.ival = 0;
		if (! strcasecmp(value, "true"))
		{
			val->value.ival = 1;
		}
		if (! strcasecmp(value, "yes"))
		{
			val->value.ival = 1;
		}
		break;
	}
	return 0;
}

int upnp_set_val_value_from_uint(upnp_val_t *val, const uint32_t value)
{
	char buf[32];

	if (! val)
	{
		return -1;
	}

	if (upnp_type_is_string_type(val->type))
	{
		snprintf(buf, sizeof(buf), "%u", value);
		return upnp_set_val_value_from_string(val, buf);
	}
	else
	{
		val->value.uval = value;
	}
	return 0;
}

int upnp_set_val_value_from_int(upnp_val_t *val, const int value)
{
	char buf[32];

	if (! val)
	{
		return -1;
	}

	if (upnp_type_is_string_type(val->type))
	{
		snprintf(buf, sizeof(buf), "%d", value);
		return upnp_set_val_value_from_string(val, buf);
	}
	else
	{
		val->value.ival = value;
	}
	return 0;
}

int upnp_set_var_value_from_uint(upnp_var_t *var, const uint32_t value)
{
	if (! var)
	{
		return -1;
	}

	return upnp_set_val_value_from_uint(&var->val, value);
}

int upnp_set_var_value_from_int(upnp_var_t *var, const int value)
{
	if (! var)
	{
		return -1;
	}

	return upnp_set_val_value_from_int(&var->val, value);
}

int upnp_set_var_value_from_string(upnp_var_t *var, const char *value)
{
	if (! var || ! value)
	{
		return -1;
	}

	return upnp_set_val_value_from_string(&var->val, value);
}

int upnp_get_val_value_as_uint(upnp_val_t *val, uint32_t *value)
{
	*value = 0;

	switch (val->type)
	{
	default:
	case upnp_dt_unknown:
	case upnp_dt_function:
		return -1;
	case upnp_dt_ui1:
	case upnp_dt_ui2:
	case upnp_dt_ui4:
		*value = val->value.uval;
		break;
	case upnp_dt_i1:
	case upnp_dt_i2:
	case upnp_dt_i4:
	case upnp_dt_int:
		*value = (uint32_t)val->value.ival;
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
		*value = (uint32_t)(char)val->value.ival;
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
		if (! val->value.sval)
		{
			return -1;
		}
		*value = (uint32_t)strtoul(val->value.sval, NULL, 0);
		break;
	case upnp_dt_bool:
		*value = val->value.ival != 0;
		break;
	}
	return 0;
}

int upnp_get_val_value_as_int(upnp_val_t *val, int *value)
{
	*value = 0;

	switch (val->type)
	{
	default:
	case upnp_dt_unknown:
	case upnp_dt_function:
		return -1;
	case upnp_dt_ui1:
	case upnp_dt_ui2:
	case upnp_dt_ui4:
		*value = (int)val->value.uval;
		break;
	case upnp_dt_i1:
	case upnp_dt_i2:
	case upnp_dt_i4:
	case upnp_dt_int:
		*value = val->value.ival;
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
		*value = (uint32_t)(char)val->value.ival;
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
		if (! val->value.sval)
		{
			return -1;
		}
		*value = strtol(val->value.sval, NULL, 0);
		break;
	case upnp_dt_bool:
		*value = val->value.ival != 0;
		break;
	}
	return 0;
}

int upnp_get_var_value_as_uint(upnp_var_t *var, uint32_t *value)
{
	if (! var || ! value)
	{
		return -1;
	}

	return upnp_get_val_value_as_uint(&var->val, value);
}

int upnp_get_var_value_as_int(upnp_var_t *var, int *value)
{
	if (! var || ! value)
	{
		return -1;
	}

	return upnp_get_val_value_as_int(&var->val, value);
}


int upnp_get_val_value_as_string(upnp_val_t *val, char *buf, size_t nbuf, const char **value, size_t *slen)
{
	if (! val)
	{
		return -1;
	}

	if (upnp_type_is_string_type(val->type))
	{
		// return a pointer to actual string if wanted
		//
		if (value)
		{
			*value = val->value.sval;
		}

		if (slen)
		{
			*slen = val->slen;
		}

		// and, if a caller supplied a buffer, copy what we can into it
		//
		if (buf)
		{
			int ncpy;

			if (val->slen >= nbuf)
			{
				ncpy = nbuf - 1;
			}
			else
			{
				ncpy = val->slen;
			}

			memcpy(buf, val->value.sval, ncpy);
			buf[ncpy] = 0;
		}
	}
	else
	{
		int nfmt;

		switch (val->type)
		{
		default:
		case upnp_dt_unknown:
		case upnp_dt_function:
			snprintf(buf, nbuf, "???");
			break;
		case upnp_dt_ui1:
			snprintf(buf, nbuf, "%u", (uint8_t)val->value.uval);
			break;
		case upnp_dt_ui2:
			snprintf(buf, nbuf, "%u", (uint16_t)val->value.uval);
			break;
		case upnp_dt_ui4:
			snprintf(buf, nbuf, "%u", (uint32_t)val->value.uval);
			break;
		case upnp_dt_i1:
			snprintf(buf, nbuf, "%d", (uint8_t)val->value.ival);
			break;
		case upnp_dt_i2:
			snprintf(buf, nbuf, "%d", (uint16_t)val->value.ival);
			break;
		case upnp_dt_i4:
		case upnp_dt_int:
			snprintf(buf, nbuf, "%d", (uint32_t)val->value.ival);
			break;
		case upnp_dt_r4:
		case upnp_dt_r8:
		case upnp_dt_number:
		case upnp_dt_fixed14:
			butil_log(2, "Unimplemented type\n");
			break;
		case upnp_dt_float:
			snprintf(buf, nbuf, "%f", val->value.fval);
			break;
		case upnp_dt_char:
			snprintf(buf, nbuf, "%c", (char)val->value.uval);
			break;
		case upnp_dt_bool:
			snprintf(buf, nbuf, "%s", (val->value.uval != 0) ? "TRUE" : "FALSE");
			break;
		}

		if (value)
		{
			*value = buf;
		}

		if (slen)
		{
			*slen = strlen(buf);
		}
	}
	return 0;
}

int upnp_get_var_value_as_string(upnp_var_t *var, char *buf, size_t nbuf, const char **value, size_t *slen)
{
	return upnp_get_val_value_as_string(&var->val, buf, nbuf, value, slen);
}

