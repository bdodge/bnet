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

#include "bmtp.h"

#define PTPFILE_DBG_LEVEL 5

static mtp_object_t s_objects[MTPD_MAX_OBJECTS];

mtp_object_t *mtpd_from_handle(uint32_t handle)
{
    size_t slot;

    if (handle < 1 || handle > MTPD_MAX_OBJECTS)
    {
        return NULL;
    }

    slot = handle - 1;
    if (s_objects[slot].valid)
    {
        return &s_objects[slot];
    }
    return NULL;

}

int mtpd_from_info(
                    uint32_t *handle,
                    uint32_t storageId,
                    const char *name,
                    uint32_t parent,
                    uint16_t format
                    )
{
    mtp_object_t *pmo;
    size_t slot;

    *handle = 0;
    for (slot = 0; slot < MTPD_MAX_OBJECTS; slot++)
    {
        pmo = &s_objects[slot];
        if (pmo->valid)
        {
            if (! strcmp(pmo->name, name))
            {
                // same name
                if (pmo->parent == parent)
                {
                    // in same directory
                    if (format == PTP_OBJFMT_ASSOC)
                    {
                        if (pmo->format == PTP_OBJFMT_ASSOC)
                        {
                            *handle = (slot + 1);
                            return 0;
                        }
                        else
                        {
                            butil_log(3, "mkdir same as existing file %s\n", name);
                            return -1;
                        }
                    }
                    else
                    {
                        if (pmo->format != PTP_OBJFMT_ASSOC)
                        {
                            *handle = (slot + 1);
                            return 0;
                        }
                        else
                        {
                            butil_log(3, "mkfile same as existing dir %s\n", name);
                            return -1;
                        }
                    }
                }
            }
        }
    }
    // no existing file
    return 1;
}

static int mtpd_getfullpath(uint32_t handle, char *path, size_t npath)
{
    mtp_object_t *pmo;
    size_t rootlen;
    uint32_t newdir;

    if (! path || (npath < MTPD_MAX_NAME))
    {
        return -1;
    }

    // put in root dir
    strncpy(path, MTPD_ROOT_DIR, npath - 3);
    path[npath - 3] = '\0';
    rootlen = strlen(path);
    path[rootlen++] = '/';
    path[rootlen] = '\0';

    // go up parent chain inserting each parent name between
    // root dir name and any existing path
    //
    newdir = handle;
    while (newdir != 0 && newdir != 0xFFFFFFFF)
    {
        pmo = mtpd_from_handle(newdir);
        if (pmo)
        {
            size_t nnlen = strlen(pmo->name);
            size_t curlen = strlen(path);
            int isdir = (pmo->format == PTP_OBJFMT_ASSOC) ? 1 : 0;

            if ((curlen + nnlen + 2 + rootlen) >= npath)
            {
                butil_log(3, "path overflow\n");
                return -1;
            }

            memmove(path + rootlen + nnlen + (isdir ? 1 : 0), path + rootlen, curlen + 1);
            memcpy(path + rootlen, pmo->name, nnlen);
            if (isdir)
            {
                path[rootlen + nnlen] = '/';
            }

            newdir = pmo->parent;
        }
    }
    return 0;
}

static int mtpd_getfile_stats(uint32_t handle)
{
    mtp_object_t *pmo;
    struct stat ostat;
    char fullpath[(MTPD_MAX_NAME + 2) * MTPD_MAX_DEPTH];

    if (mtpd_getfullpath(handle, fullpath, sizeof(fullpath)))
    {
        return -1;
    }

    if (stat(fullpath, &ostat) < 0)
    {
        return -1;
    }

    pmo = mtpd_from_handle(handle);
    if (pmo)
    {
        pmo->size = ostat.st_size;
        pmo->modt = ostat.st_mtime;
    }
    return 0;
}

uint16_t mtpd_format_from_name(const char *name)
{
    const char *ext;
    int len;

    if (! name || ! *name)
    {
        return PTP_OBJFMT_MP3;
    }

    len = strlen(name);
    for (ext = name + len - 1; ext >= name; ext--)
    {
        if (*ext == '.')
        {
            break;
        }
    }
    if (*ext != '.')
    {
        return PTP_OBJFMT_MP3;
    }

    ext++;
    if (! strncasecmp(ext, "htm", 3) || ! strncasecmp(ext, "html", 4))
    {
        return PTP_OBJFMT_HTML;
    }
    /*
    if (! strncasecmp(ext, "jpg", 3) || ! strncasecmp(ext, "jpeg", 4))
    {
        return PTP_OBJFMT_EXIF_JPEG;
    }
    if (! strncasecmp(ext, "gif", 3))
    {
        return PTP_OBJFMT_GIF;
    }
    */
    if (! strncasecmp(ext, "mp3", 3))
    {
        return PTP_OBJFMT_MP3;
    }
    if (! strncasecmp(ext, "m3u", 3))
    {
        return PTP_OBJFMT_M3UPLAYLIST;
    }
    if (! strncasecmp(ext, "xml", 3))
    {
        return PTP_OBJFMT_XMLDOCUMENT;
    }
    return PTP_OBJFMT_MP3;
}

int mtpd_createfile(
                    const char *name,
                    uint32_t parent,
                    uint16_t format,
                    uint32_t size,
                    uint16_t access,
                    uint32_t *handle
                   )
{
    size_t slot;

    for (slot = 0; slot < MTPD_MAX_OBJECTS; slot++)
    {
        if (! s_objects[slot].valid)
        {
            break;
        }
    }

    if (slot >= MTPD_MAX_OBJECTS)
    {
        return -1;
    }

    if (format == 0)
    {
        format = mtpd_format_from_name(name);
    }

    s_objects[slot].valid  = 1;
    s_objects[slot].parent = parent;
    s_objects[slot].format = format;
    s_objects[slot].access = access;
    s_objects[slot].size   = size;
    strncpy(s_objects[slot].name, name, MTPD_MAX_NAME - 1);
    s_objects[slot].name[MTPD_MAX_NAME - 1] = '\0';
    if (handle)
    {
        *handle = slot + 1;
    }

    if (size == 0)
    {
        mtpd_getfile_stats(slot + 1);
    }
    return 0;
}

int mtpd_destroyfile(uint32_t handle)
{
    mtp_object_t *pmo;

    pmo = mtpd_from_handle(handle);
    if (! pmo)
    {
        return -1;
    }

    if (pmo->fd >= 0)
    {
        close(pmo->fd);
        pmo->fd = -1;
    }
    pmo->valid = 0;
    pmo->name[0] = 0;
    pmo->modt = 0;
    return 0;
}

int mtpd_makedir(uint32_t handle)
{
    mtp_object_t *pmo;
    char fullpath[(MTPD_MAX_NAME + 2) * MTPD_MAX_DEPTH];
    int rc;

    pmo = mtpd_from_handle(handle);
    if (! pmo)
    {
        return -1;
    }

    if (pmo->fd >= 0)
    {
        return 0;
    }

    if (mtpd_getfullpath(handle, fullpath, sizeof(fullpath)))
    {
        return -1;
    }

    rc = mkdir(fullpath, 0755);
    return rc;
}

int mtpd_openfile(uint32_t handle, int rw)
{
    mtp_object_t *pmo;
    char fullpath[(MTPD_MAX_NAME + 2) * MTPD_MAX_DEPTH];

    pmo = mtpd_from_handle(handle);
    if (! pmo)
    {
        return -1;
    }

    if (pmo->fd >= 0)
    {
        return 0;
    }

    if (mtpd_getfullpath(handle, fullpath, sizeof(fullpath)))
    {
        return -1;
    }

    pmo->fd = open(fullpath, rw ? (O_WRONLY | O_CREAT | O_TRUNC) : (O_RDONLY), 0644);
    if (pmo->fd < 0)
    {
        return -1;
    }
    return 0;
}

int mtpd_deletefile(uint32_t handle)
{
    mtp_object_t *pmo;

    pmo = mtpd_from_handle(handle);
    if (! pmo)
    {
        return -1;
    }

    butil_log(3, "Not actually unlinking %s\n", pmo->name);
    mtpd_destroyfile(handle);
    return 0;
}

int mtpd_appendfile(uint32_t handle, uint8_t *data, size_t count)
{
    mtp_object_t *pmo;
    int wc, wt;

    pmo = mtpd_from_handle(handle);
    if (! pmo)
    {
        return -1;
    }

    if (pmo->fd < 0)
    {
        return 0;
    }

    for (wt = 0; wt < count;)
    {
        wc = write(pmo->fd, data, count - wt);
        if (wc < 0) {
            return wc;
        }
        wt += wc;
    }
    pmo->size += wt;
    return 0;
}

int mtpd_readfile(uint32_t handle, uint8_t *data, size_t count, size_t *readcount)
{
    mtp_object_t *pmo;
    int rc, rt;

    pmo = mtpd_from_handle(handle);
    if (! pmo)
    {
        return -1;
    }

    if (pmo->fd < 0)
    {
        return 0;
    }

    *readcount = 0;
    if (pmo->size == 0)
    {
        return 0;
    }

    rt = 0;
    do
    {
        rc = read(pmo->fd, data, count - rt);
        if (rc < 0)
        {
            return rc;
        }

        rt += rc;
    }
    while (rt < count && rc < 0);
    *readcount = rt;
    return 0;
}

int mtpd_closefile(uint32_t handle)
{
    mtp_object_t *pmo;

    pmo = mtpd_from_handle(handle);
    if (! pmo)
    {
        return -1;
    }

    if (pmo->fd < 0)
    {
        return 0;
    }

    close(pmo->fd);
    pmo->fd = -1;
    if (pmo->access == PTP_OBJ_READWRITE)
    {
        // update size if writing, just to make sure disk size is consistent
        // with written size?
        mtpd_getfile_stats(handle);
    }
    return 0;
}

uint32_t mtpd_file_handle(size_t index)
{
    size_t slot;
    size_t count;

    for (slot = count = 0; slot < MTPD_MAX_OBJECTS; slot++)
    {
        if (s_objects[slot].valid)
        {
            if (count >= index)
            {
                return slot + 1;
            }
            count++;
        }
    }
    return 0;
}

size_t mtpd_file_count()
{
    size_t slot;
    size_t count;

    for (slot = count = 0; slot < MTPD_MAX_OBJECTS; slot++)
    {
        if (s_objects[slot].valid)
        {
            count++;
        }
    }
    return count;
}

mtp_object_t *mtpd_file_info(uint32_t handle)
{
    mtp_object_t *pmo;

    pmo = mtpd_from_handle(handle);
    return pmo;
}

static int mtpd_onenumcreate(
                            const char *name,
                            uint32_t parent,
                            uint16_t format,
                            uint32_t size,
                            uint16_t access
                            )
{
    int rc;

    if (PTPFILE_DBG_LEVEL > 0)
    {
        butil_log(3, "enum create %s\n", name);
    }

    rc = mtpd_createfile(name, parent, format, size, access, NULL);
    return rc;
}

int mtpd_enumfiles(uint32_t parent, int level, int maxlevel,
    int (*enumfunc)(const char *name, uint32_t parent, uint16_t format, uint32_t size, uint16_t access))
{
    DIR *dir;
    struct dirent *dent;
    uint32_t newdir;
    int rc;

    char dirname[(MTPD_MAX_NAME + 4) * MTPD_MAX_DEPTH];

    if (mtpd_getfullpath(parent, dirname, sizeof(dirname))) {
        return -1;
    }
    if (PTPFILE_DBG_LEVEL > 1)
    {
        butil_log(3, "explore dir %s\n", dirname);
    }
    dir = opendir(dirname);
    if (dir)
    {
        do
        {
            dent = readdir(dir);
            if (! dent)
            {
                break;
            }

            if (dent->d_type == DT_REG)
            {
                rc = enumfunc(dent->d_name, parent, 0, PTP_OBJ_READWRITE, 0);
            }
            else if (dent->d_type == DT_DIR)
            {
                if (strcmp(dent->d_name, "..") && strcmp(dent->d_name, "."))
                {
                    // create a directory node and recurse
                    rc = mtpd_createfile(dent->d_name, parent, PTP_OBJFMT_ASSOC, 0, PTP_OBJ_READWRITE, &newdir);
                    if (! rc)
                    {
                        rc = mtpd_enumfiles(newdir, level + 1, maxlevel, enumfunc);
                    }
                }
            }
        }
        while (dent);

        closedir(dir);
    }
    return 0;
}

int mtpd_files_init()
{
    size_t i;

    for (i = 0; i < MTPD_MAX_OBJECTS; i++)
    {
        s_objects[i].valid = 0;
        s_objects[i].fd = -1;
    }
#if 0
    // create 2 existing objects for testing
    //
    mtpd_createfile("test1.mp3", 0xFFFFFFFF, PTP_OBJFMT_MP3, 0, PTP_OBJ_READONLY, NULL);
    mtpd_createfile("test2.mp3", 0xFFFFFFFF, PTP_OBJFMT_MP3, 0, PTP_OBJ_READONLY, NULL);
#else
    // make sure "store" exist
    {
    struct stat ostat;

    if (stat("store", &ostat) < 0) {
        butil_log(3, "no store directory, creating ./store");
        mkdir("store", 0775);
    }
    }
    // list "store" directory as root, recurse
    mtpd_enumfiles(0xFFFFFFFF, 0, MTPD_MAX_DEPTH, mtpd_onenumcreate);
#endif
    return 0;
}

