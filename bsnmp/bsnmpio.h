#ifndef SNMPIO_H
#define SNMPIO_H 1

#include "bsnmp.h"

int bsnmp_next_byte                 (bsnmp_server_t *server, uint8_t *b);
int bsnmp_out_byte                  (bsnmp_server_t *server, const uint8_t b);
int bsnmp_stream_save_in_state      (bsnmp_server_t *server, ioring_t *iostate);
int bsnmp_stream_restore_in_state   (bsnmp_server_t *server, ioring_t *iostate);
int bsnmp_stream_save_out_state     (bsnmp_server_t *server, ioring_t *iostate);
int bsnmp_stream_restore_out_state  (bsnmp_server_t *server, ioring_t *iostate);
int bsnmp_stream_annotate_length    (bsnmp_server_t *server, ioring_t *frompos, uint32_t bytes_reserved);
int bsnmp_stream_init               (bsnmp_server_t *server, uint16_t version, uint16_t port);
int bsnmp_stream_read               (bsnmp_server_t *server, int tos, int tous);
int bsnmp_stream_write              (bsnmp_server_t *server, int tos, int tous);

#endif
