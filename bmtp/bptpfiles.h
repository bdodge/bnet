/*
 * Copyright 2020 Brian Dodge
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

#ifndef BPTPFILES_H
#define BPTPFILES_H 1

#define MTPD_ROOT_DIR       "store"

#define MTPD_ROOT_STORE     0x00010001
#define MTPD_MAX_NAME       64
#define MTPD_MAX_OBJECTS    16
#define MTPD_MAX_DEPTH      5

typedef struct mtp_object
{
    uint32_t    parent;
    char        name[MTPD_MAX_NAME];
    size_t      size;
    time_t      modt;
    uint16_t    format;
    uint16_t    access;
    int         valid;
    int         fd;
}
mtp_object_t;

uint16_t     mtpd_format_from_name(const char *name);
mtp_object_t *mtpd_from_handle(uint32_t handle);
int          mtpd_from_info(
                            uint32_t *handle,
                            uint32_t storageId,
                            const char *name,
                            uint32_t parent,
                            uint16_t format
                            );
int          mtpd_createfile(
                            const char *name,
                            uint32_t parent,
                            uint16_t format,
                            uint32_t size,
                            uint16_t access,
                            uint32_t *handle
                            );
int          mtpd_destroyfile(uint32_t handle);
int          mtpd_makedir(uint32_t handle);
int          mtpd_openfile(uint32_t handle, int rw);
int          mtpd_appendfile(uint32_t handle, uint8_t *data, size_t count);
int          mtpd_readfile(uint32_t handle, uint8_t *data, size_t count, size_t *readcount);
int          mtpd_closefile(uint32_t handle);
int          mtpd_deletefile(uint32_t handle);
mtp_object_t *mtpd_file_info(uint32_t handle);
uint32_t     mtpd_file_handle(size_t index);
size_t       mtpd_file_count();
int          mtpd_files_init(void);

#endif


