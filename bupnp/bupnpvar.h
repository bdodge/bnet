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
#ifndef BUPNPVAR_H
#define BUPNPVAR_H 1

typedef enum
{
    upnp_dt_unknown,
    upnp_dt_function,
    upnp_dt_ui1,
    upnp_dt_ui2,
    upnp_dt_ui4,
    upnp_dt_i1,
    upnp_dt_i2,
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

typedef struct upnp_val
{
    upnp_vartype_t  type;
    size_t          alloclen;
    size_t          slen;
    union
    {
        char    *sval;
        uint32_t uval;
        int32_t  ival;
        double   fval;
    }
    value;
}
upnp_val_t;

typedef struct upnp_var
{
    char               *name;
    upnp_val_t          val;
    struct upnp_var    *next;
}
upnp_var_t;

upnp_vartype_t upnp_type_string_to_type(const char *typestr);
bool upnp_type_is_string_type(const upnp_vartype_t type);

int upnp_set_val_value_from_uint(upnp_val_t *val, const uint32_t value);
int upnp_set_val_value_from_int(upnp_val_t *val, const int value);
int upnp_set_var_value_from_uint(upnp_var_t *var, const uint32_t value);
int upnp_set_var_value_from_int(upnp_var_t *var, const int value);
int upnp_set_val_value_from_string(upnp_val_t *val, const char *value);
int upnp_set_var_value_from_string(upnp_var_t *var, const char *value);

int upnp_get_val_valus_as_uint(upnp_val_t *val, uint32_t *value);
int upnp_get_val_value_as_int(upnp_val_t *val, int *value);
int upnp_get_var_valus_as_uint(upnp_var_t *var, uint32_t *value);
int upnp_get_var_value_as_int(upnp_var_t *var, int *value);
int upnp_get_val_value_as_string(upnp_val_t *val, char *buf, size_t nbuf, const char **value, size_t *slen);
int upnp_get_var_value_as_string(upnp_var_t *var, char *buf, size_t nbuf, const char **value, size_t *slen);


#endif

