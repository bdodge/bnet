#ifndef HTTP_WS_H
#define HTTP_WS_H 1

#define HTTP_MAX_WEBSOCKET_KEY  32
#define HTTP_MAX_WEBSOCKET_PROTOCOL  32
#define HTTP_WEBSOCKET_KEY_UUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

typedef enum
{
    wssFrame,
    wssHeaderByte1,
    wssPayloadLen1,
    wssPayloadLen2,
    wssMaskingKey,
    wssPayload
}
http_ws_state_t;

typedef struct
{
    http_ws_state_t in_state;
    http_ws_state_t out_state;
    http_ws_state_t prev_in_state;
    http_ws_state_t prev_out_state;
    uint8_t         opcode;
    uint64_t        payload_length;
    uint8_t         mask_key[4];
    iostream_t     *stream;
    ioring_t        in;
}
ws_stream_ctx_t;

int http_websocket_upgrade_reply(struct http_client *client);
int http_websocket_slice(struct http_client *client);

#endif
