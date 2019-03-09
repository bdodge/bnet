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
#include "bsnmp.h"
#include "bsnmpvar.h"
#include "bsnmputils.h"
#include "bsnmpobject.h"
#include "butil.h"
#include "bnetheaders.h"

static bsnmp_oid_t s_test_oid1 = { 3, { 0, 1, 2 } };
static uint32_t s_test_val1 = 1234;

static bsnmp_oid_t s_test_oid2 = { 3, { 0, 1, 3 } };
static char s_test_val2[32] = "This is a test";

static bsnmp_oid_t s_test_oid3 = { 3, { 0, 1, 4 } };
static bsnmp_oid_t s_test_val3 = { 4, { 9, 10, 65531, 1 } };

static int callback(bsnmp_server_t *server, bsnmp_request_t *req, bsnmp_var_t *var)
{
    char debug_buffer[128];
    int result;

    butil_log(5, "  - %s %ub %s\n", bsnmp_opcode_string(req->code),
            req->len, bsnmp_oid_string(&var->oid, debug_buffer, sizeof(debug_buffer)));
    butil_log(5, "    . %s %s\n", bsnmp_type_string(var->type),
            bsnmp_var_string(var, debug_buffer, sizeof(debug_buffer)));

    req->errmsg = SNMP_ErrNoSuchName;
    req->errdex = 0;

    if (bsnmp_oidcmp(&var->oid, &s_test_oid1, NULL) == snmpCmpExact)
    {
        switch (req->code)
        {
        case SNMP_GET:
            req->errmsg = SNMP_ErrNoError;
            var->type = SNMP_UNSIGNED;
            var->val.uVal = s_test_val1;
            break;
        case SNMP_SET:
            req->errmsg = SNMP_ErrNoError;
            s_test_val1 = var->val.uVal;
            break;
        default:
            req->errmsg = SNMP_ErrNoAccess;
            break;
        }
    }
    else if (bsnmp_oidcmp(&var->oid, &s_test_oid2, NULL) == snmpCmpExact)
    {
        switch (req->code)
        {
        case SNMP_GET:
            req->errmsg = SNMP_ErrNoError;
            var->type = SNMP_OCTET_STRING;
            var->val.sVal = s_test_val2;
            var->len = strlen(s_test_val2);
            var->alloc_len = 0;
            break;
        case SNMP_SET:
            if (var->type != SNMP_OCTET_STRING)
            {
                req->errmsg = SNMP_ErrWrongType;
            }
            else if (var->len >= sizeof s_test_val2)
            {
                req->errmsg= SNMP_ErrTooBig;
            }
            else
            {
                req->errmsg = SNMP_ErrNoError;
                memcpy(s_test_val2, var->val.sVal, var->len);
                s_test_val2[var->len] = 0;
            }
            break;
        default:
            req->errmsg = SNMP_ErrNoAccess;
            break;
        }
    }
    else if (bsnmp_oidcmp(&var->oid, &s_test_oid3, NULL) == snmpCmpExact)
    {
        switch (req->code)
        {
        case SNMP_GET:
            req->errmsg = SNMP_ErrNoError;
            var->type = SNMP_OBJECT_ID;
            var->val.oVal = &s_test_val3;
            var->len = s_test_val3.len;
            var->alloc_len = 0;
            break;
        case SNMP_SET:
            if (var->type != SNMP_OBJECT_ID)
            {
                req->errmsg = SNMP_ErrWrongType;
            }
            else
            {
                req->errmsg = SNMP_ErrNoError;
                bsnmp_oid_copy(&s_test_val3, var->val.oVal);
            }
            break;
        default:
            req->errmsg = SNMP_ErrNoAccess;
            break;
        }
    }
    else
    {
        switch (req->code)
        {
        case SNMP_GET:
            req->errmsg = SNMP_ErrNoError;
            result = bsnmp_get_object_value(&var->oid, var, &req->errmsg);
            break;
        case SNMP_GETNEXT:
            req->errmsg = SNMP_ErrNoError;
            result = bsnmp_get_next_object_value(&var->oid, var, &req->errmsg);
            break;
        case SNMP_SET:
            req->errmsg = SNMP_ErrNoAccess;
            break;
        default:
            req->errmsg = SNMP_ErrNoAccess;
            break;
        }
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    bsnmp_server_t server;
    uint16_t port;
    int result;

    butil_set_log_level(5);

    port = SNMP_AGENT_PORT;
    result = bsnmp_server_init(&server, SNMP_V2C, port, callback);
    if (result)
    {
        butil_log(0, "can't start server");
    }
    else
    {
        butil_log(1, "serving SNMP on port %u\n", server.port);

        result = bsnmp_serve(&server);

        butil_log(1, "server on port %u ends (%d)\n", server.port, result);
    }
    bsnmp_server_cleanup(&server);
    return result;
}

