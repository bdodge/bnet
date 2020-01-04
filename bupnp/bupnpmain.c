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

#define UUID_STRING "01020304-0506-0708-0900-010203040506"

static uuid_t uuid_device = { 1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6 };

static char s_device_description[];
static char s_content_directory_scpd[];
static char s_connection_manager_scpd[];

int content_directory_action(upnp_server_t *server, upnp_service_t *service, const char *action)
{
    uint32_t objID;
    int startIndex;
    int requestedCount;
    int result;

    butil_log(3, "ContentDirectory Action: %s\n", action);
    butil_log(3, (char*)server->soap.data);

    if (! strcmp(action, "Browse"))
    {
        // parse input arguments
        //
        result = upnp_get_arg_value_as_int(service, action, "ObjectID", (int*)&objID);
        if (result)
        {
            butil_log(2, "Missing ObjectID in Browse\n");
            return -1;
        }

        result = upnp_get_arg_value_as_int(service, action, "StartingIndex", &startIndex);
        if (result)
        {
            butil_log(2, "Missing StartingIndex in Browse\n");
            return -1;
        }

        result = upnp_get_arg_value_as_int(service, action, "RequestedCount", &requestedCount);
        if (result)
        {
            butil_log(4, "Missing RequestedCount in Browse\n");
            requestedCount = 0xFFFFFFFF;
            result = 0;
        }

        butil_log(3, "Browse ID:%08X start=%u count=%u\n", objID, startIndex, requestedCount);

        // set output arguments
        //
        upnp_set_arg_value_from_int(service, action, "NumberReturned", 0);
        upnp_set_arg_value_from_int(service, action, "TotalMatches", 0);
    }
    return 0;
}

int main(int argc, char **argv)
{
    bool isserver;
    uint16_t port;
    char url[HTTP_MAX_LINE];
    const char *program, *arg;
    int loglevel = 5;
    int result;

#ifdef Windows
    WSADATA wsaData;

    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        UPNP_ERROR("WSAStartup failed");
        return -1;
    }
#else
    signal(SIGPIPE, SIG_IGN);
#endif
    butil_set_log_level(loglevel);

#if UPNP_SUPPORT_TLS
    result = iostream_tls_prolog();
    if (result)
    {
        UPNP_ERROR("Can't init TLS");
        return -1;
    }
#endif

    program = *argv++;
    argc--;

    isserver = true;
    loglevel = 3;

    port = 8080;
    result = 0;

    while (argc > 0 && ! result)
    {
        arg = *argv++;
        argc--;
        if (arg[0] == '-')
        {
            switch (arg[1])
            {
            case 'X':
                isserver = false;
                break;
            case 'p':
                if (argc > 0)
                {
                    port = strtoul(*argv, NULL, 0);
                    argv++;
                    argc--;
                }
                else
                {
                    butil_log(0, "Use: -p [port]");
                }
                break;
            case 'l':
                if (argc > 0)
                {
                    loglevel = strtoul(*argv, NULL, 0);
                    argv++;
                    argc--;
                }
                else
                {
                    http_log(0, "Use: -l [level]");
                }
                break;
            default:
                http_log(0, "Bad Switch: %s\n", arg);
                break;
            }
        }
        else
        {
            strncpy(url, arg, sizeof(url) - 1);
            url[sizeof(url) - 1] = '\0';
        }
    }
    butil_set_log_level(loglevel);

    if (isserver)
    {
        upnp_server_t server;

        result = upnp_server_init(
                                    &server,
                                    65536,
                                    port,
                                    uuid_device,
                                    "/device_description",
                                    "MediaServer",
                                    1,
                                    2000
                                );
        if (result)
        {
            UPNP_ERROR("can't init server");
            return result;
        }

        result = upnp_add_text_url(&server, "/device_description", butil_mime_xml, s_device_description);
        if (result)
        {
            UPNP_ERROR("can't add content scpd url");
            return result;
        }

        // add a content directory service
        //
        result = upnp_add_service(
                                    &server,
                                    server.root_device,
                                    content_directory_action,
                                    "ContentDirectory",
                                    1,
                                    "/content_scpd",
                                    s_content_directory_scpd,
                                    "/content_control",
                                    "/content_event"
                                 );

        if (result)
        {
            UPNP_ERROR("can't add content service");
            return result;
        }

        // add a connection manager service
        //
        result = upnp_add_service(
                                    &server,
                                    server.root_device,
                                    NULL, //on_conman,
                                    "ConnectionManager",
                                    1,
                                    "/conman_scpd",
                                    s_connection_manager_scpd,
                                    "/conman_control",
                                    "/conman_event"
                                 );

        if (result)
        {
            UPNP_ERROR("can't add conman service");
            return result;
        }

        result = upnp_serve(&server, NULL, NULL);
        if (result)
        {
            butil_log(2, "server on port %u ends\n", server.port);
        }

        upnp_server_cleanup(&server);
    }
    return result;
}

static char s_device_description[] =
"<?xml version=\"1.0\"?>\n"
"<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n"
"<specVersion>\n"
"<major>1</major>\n"
"<minor>0</minor>\n"
"</specVersion>\n"
//"<URLBase>/</URLBase>\n"
"<device>\n"
"<deviceType>urn:schemas-upnp-org:device:MediaServer:1</deviceType>\n"
"<friendlyName>Bnet Media Server</friendlyName>\n"
"<manufacturer>Brian Dodge</manufacturer>\n"
"<manufacturerURL>http://www.hell.org/</manufacturerURL>\n"
"<modelDescription>Test of Bnet UPnP server system</modelDescription>\n"
"<modelName>bnet</modelName>\n"
"<modelNumber>42</modelNumber>\n"
"<modelURL>http://www.hell.org/</modelURL>\n"
"<serialNumber>12345</serialNumber>\n"
"<UDN>uuid:" UUID_STRING "</UDN>\n"

//"<presentationURL>canned.html</presentationURL>\n"
//"<UPC>Universal Product Code</UPC>\n"

/*
"<iconList>\n"
"<icon>\n"
"<mimetype>image/format</mimetype>\n"
"<width>64</width>\n"
"<height>64</height>\n"
"<depth>24</depth>\n"
"<url></url>\n"
"</icon>\n"
"</iconList>\n"
*/
"<serviceList>\n"
"<service>\n"
"<serviceType>urn:schemas-upnp-org:service:ContentDirectory:1</serviceType>\n"
"<serviceId>urn:upnp-org:serviceId:ContentDirectory</serviceId>\n"
"<SCPDURL>content_scpd</SCPDURL>\n"
"<controlURL>content_control</controlURL>\n"
"<eventSubURL>content_event</eventSubURL>\n"
"</service>\n"
"<service>\n"
"<serviceType>urn:schemas-upnp-org:service:ConnectionManager:1</serviceType>\n"
"<serviceId>urn:upnp-org:serviceId:ConnectionManager</serviceId>\n"
"<SCPDURL>conman_scpd</SCPDURL>\n"
"<controlURL>conman_control</controlURL>\n"
"<eventSubURL>conman_event</eventSubURL>\n"
"</service>\n"
/*
"<service>\n"
"<serviceType>urn:schemas-upnp-org:service:AVTransport:1</serviceType>\n"
"<serviceId>urn:upnp-org:serviceId:AVTransport</serviceId>\n"
"<SCPDURL>transport_scpd</SCPDURL>\n"
"<controlURL>transport_control</controlURL>\n"
"<eventSubURL>transport_event</eventSubURL>\n"
"</service>\n"
*/
"</serviceList>\n"
"</device>\n"
"</root>\n";

static char s_content_directory_scpd[] =
"<?xml version=\"1.0\"?>\n"
"<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">\n"
"<specVersion>\n"
"<major>1</major>\n"
"<minor>0</minor>\n"
"</specVersion>\n"
"<actionList>\n"
"<action>\n"
"<name>GetSearchCapabilities</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>SearchCaps</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"SearchCapabilities\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>GetSortCapabilities</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>SortCaps</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"SortCapabilities\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>GetSortExtensionCapabilities</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>SortExtensionCaps</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"SortExtensionCapabilities\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>GetFeatureList</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>FeatureList</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"FeatureList\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>GetSystemUpdateID</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>Id</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"SystemUpdateID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>Browse</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>ObjectID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ObjectID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>BrowseFlag</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_BrowseFlag\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>Filter</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Filter\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>StartingIndex</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Index\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>RequestedCount</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Count\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>SortCriteria</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_SortCriteria\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>Result</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Result\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>NumberReturned</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Count\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>TotalMatches</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Count\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>UpdateID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_UpdateID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>Search</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>ContainerID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ObjectID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>SearchCriteria</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_SearchCriteria\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>Filter</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Filter\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>StartingIndex</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Index\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>RequestedCount</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Count\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>SortCriteria</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_SortCriteria\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>Result</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Result\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>NumberReturned</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Count\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>TotalMatches</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Count\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>UpdateID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_UpdateID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>CreateObject</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>ContainerID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ObjectID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>Elements</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Result\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>ObjectID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ObjectID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>Result</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Result\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>DestroyObject</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>ObjectID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ObjectID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>UpdateObject</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>ObjectID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ObjectID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>CurrentTagValue</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_TagValueList\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>NewTagValue</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_TagValueList\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>MoveObject</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>ObjectID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ObjectID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>NewParentID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ObjectID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>NewObjectID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ObjectID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>ImportResource</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>SourceURI</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_URI\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>DestinationURI</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_URI\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>TransferID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_TransferID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>ExportResource</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>SourceURI</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_URI\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>DestinationURI</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_URI\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>TransferID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_TransferID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>StopTransferResource</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>TransferID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_TransferID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>DeleteResource</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>ResourceURI</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_URI\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>GetTransferProgress</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>TransferID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_TransferID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>TransferStatus</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_TransferStatus\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>TransferLength</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_TransferLength\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>TransferTotal</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_TransferTotal\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>CreateReference</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>ContainerID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ObjectID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>ObjectID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ObjectID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>NewID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ObjectID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"</actionList>\n"
"<serviceStateTable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>SearchCapabilities</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>SortCapabilities</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>SortExtensionCapabilities</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"yes\">\n"
"<name>SystemUpdateID</name>\n"
"<dataType>ui4</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"yes\">\n"
"<name>ContainerUpdateIDs</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"yes\">\n"
"<name>TransferIDs</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>FeatureList</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_ObjectID</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_Result</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_SearchCriteria</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_BrowseFlag</name>\n"
"<dataType>string</dataType>\n"
"<allowedValueList>\n"
"<allowedValue>BrowseMetadata</allowedValue>\n"
"<allowedValue>BrowseDirectChildren</allowedValue>\n"
"</allowedValueList>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_Filter</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_SortCriteria</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_Index</name>\n"
"<dataType>ui4</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_Count</name>\n"
"<dataType>ui4</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_UpdateID</name>\n"
"<dataType>ui4</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_TransferID</name>\n"
"<dataType>ui4</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_TransferStatus</name>\n"
"<dataType>string</dataType>\n"
"<allowedValueList>\n"
"<allowedValue>COMPLETED</allowedValue>\n"
"<allowedValue>ERROR</allowedValue>\n"
"<allowedValue>IN_PROGRESS</allowedValue>\n"
"<allowedValue>STOPPED</allowedValue>\n"
"</allowedValueList>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_TransferLength</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_TransferTotal</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_TagValueList</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_URI</name>\n"
"<dataType>uri</dataType>\n"
"</stateVariable>\n"
"</serviceStateTable>\n"
"</scpd>\n";

static char s_connection_manager_scpd[] =
"<?xml version=\"1.0\"?>\n"
"<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">\n"
"<specVersion>\n"
"<major>1</major>\n"
"<minor>0</minor>\n"
"</specVersion>\n"
"<actionList>\n"
"<action>\n"
"<name>GetProtocolInfo</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>Source</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"SourceProtocolInfo\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>Sink</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"SinkProtocolInfo\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>PrepareForConnection</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>RemoteProtocolInfo</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ProtocolInfo\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>PeerConnectionManager</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ConnectionManager\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>PeerConnectionID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ConnectionID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>Direction</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Direction\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>ConnectionID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ConnectionID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>AVTransportID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_AVTransportID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>RcsID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_RcsID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>ConnectionComplete</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>ConnectionID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ConnectionID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>GetCurrentConnectionIDs</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>ConnectionIDs</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"CurrentConnectionIDs\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"<action>\n"
"<name>GetCurrentConnectionInfo</name>\n"
"<argumentList>\n"
"<argument>\n"
"<name>ConnectionID</name>\n"
"<direction>in</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ConnectionID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>RcsID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_RcsID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>AVTransportID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_AVTransportID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>ProtocolInfo</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ProtocolInfo\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>PeerConnectionManager</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ConnectionManager\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>PeerConnectionID</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ConnectionID\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>Direction</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_Direction\n"
"</relatedStateVariable>\n"
"</argument>\n"
"<argument>\n"
"<name>Status</name>\n"
"<direction>out</direction>\n"
"<relatedStateVariable>\n"
"A_ARG_TYPE_ConnectionStatus\n"
"</relatedStateVariable>\n"
"</argument>\n"
"</argumentList>\n"
"</action>\n"
"</actionList>\n"
"<serviceStateTable>\n"
"<stateVariable sendEvents=\"yes\">\n"
"<name>SourceProtocolInfo</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"yes\">\n"
"<name>SinkProtocolInfo</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"yes\">\n"
"<name>CurrentConnectionIDs</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_ConnectionStatus</name>\n"
"<dataType>string</dataType>\n"
"<allowedValueList>\n"
"<allowedValue>OK</allowedValue>\n"
"<allowedValue>ContentFormatMismatch</allowedValue>\n"
"<allowedValue>InsufficientBandwidth</allowedValue>\n"
"<allowedValue>UnreliableChannel</allowedValue>\n"
"<allowedValue>Unknown</allowedValue>\n"
"</allowedValueList>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_ConnectionManager</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_Direction</name>\n"
"<dataType>string</dataType>\n"
"<allowedValueList>\n"
"<allowedValue>Input</allowedValue>\n"
"<allowedValue>Output</allowedValue>\n"
"</allowedValueList>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_ProtocolInfo</name>\n"
"<dataType>string</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_ConnectionID</name>\n"
"<dataType>i4</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_AVTransportID</name>\n"
"<dataType>i4</dataType>\n"
"</stateVariable>\n"
"<stateVariable sendEvents=\"no\">\n"
"<name>A_ARG_TYPE_RcsID</name>\n"
"<dataType>i4</dataType>\n"
"</stateVariable>\n"
"</serviceStateTable>\n"
"</scpd>\n";

