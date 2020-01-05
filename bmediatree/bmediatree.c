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
#include "bmediatree.h"

static int s_invert_format_priorities = 0;

#define MEDIAALLOC malloc
#define MEDIAFREE free

void media_tree_dump(media_tree_t *tree, media_obj_t *pm)
{
    media_obj_t *px, *pz;
    char    cl[128];
    int     i, level;

    pz = pm;
    if(! pz)
    {
        return;
    }

    level = 0;

    do
    {
        px = pz;

        for(i = 0; i <= level && i < 16; i++)
        {
            sprintf(cl + i*2, (px == tree->current) ? "--" : "  ");
        }
		butil_log(2, "%s%s\n", cl, px->title ? px->title : "<nil>");

        if(pz->child)
        {
            level++;
            pz = pz->child;
        }
        else if(pz->sibling)
        {
            pz = pz->sibling;
        }
        else
        {
            do
            {
                pz = pz->parent;
                level--;
                if(pz && pz->sibling)
                {
                    pz = pz->sibling;
                    break;
                }
            }
            while(pz);
        }
    }
    while(pz);
}

size_t media_child_count(media_obj_t *pm)
{
    size_t count = 0;

    if (! pm)
    {
        return 0;
    }

    if (! pm->child)
    {
        return 0;
    }

    for (pm = pm->child; pm; pm = pm->sibling)
    {
        count++;
    }

    return count;
}

size_t media_tree_count(media_tree_t *tree)
{
    media_obj_t *px;
    media_obj_t *pz;
    size_t count = 0;

    pz = tree->root;
    if(! pz)
    {
        return 0;
    }

    do
    {
        px = pz;
        count++;

        if(pz->child)
        {
            pz = pz->child;
        }
        else if(pz->sibling)
        {
            pz = pz->sibling;
        }
        else
        {
            do
            {
                pz = pz->parent;
                if(pz && pz->sibling)
                {
                    pz = pz->sibling;
                    break;
                }
            }
            while(pz);
        }
    }
    while(pz);

    return count;
}

media_tree_t *media_tree_create(void)
{
	media_tree_t *tree;

	tree = (media_tree_t *)MEDIAALLOC(sizeof(media_tree_t));
	if (! tree)
	{
		return NULL;
	}

	tree->media = NULL;
	tree->root  = NULL;
	tree->current = NULL;

	return tree;
}

void media_tree_destroy(media_tree_t *tree)
{
	media_obj_t *pm;

	if (! tree)
	{
		return;
	}

	tree->current = NULL;
	tree->root = NULL;

	while (tree->media)
	{
		pm = tree->media;
		tree->media = pm->next;
		media_delete(pm);
	}
	MEDIAFREE(tree);
}

media_obj_t *media_create(const char *id, int kidnum, media_obj_type_t type, const char *name)
{
    media_obj_t *pm;
    int    len;

    pm = (media_obj_t *)MEDIAALLOC(sizeof(media_obj_t));

    if(pm)
    {
        pm->type = type;
        len = strlen(id) + 1;

        pm->id = (char*)MEDIAALLOC(len);
        if(! pm->id)
        {
            MEDIAFREE(pm);
            return NULL;
        }

        strcpy(pm->id, id);

        len = strlen(name) + 1;
        pm->title = (char*)MEDIAALLOC(len);
        if(! pm->title)
        {
            MEDIAFREE(pm->id);
            MEDIAFREE(pm);
            return NULL;
        }

        strcpy(pm->title, name);

        pm->subtitle    = NULL;
        pm->artist      = NULL;
        pm->album       = NULL;
        pm->url         = NULL;
        pm->iurl        = NULL;
        pm->reliability = -1;

        pm->bits = 0;
        pm->chan = 0;
        pm->rate = 0;
        pm->brt  = 0;
        pm->codec = butil_mime_bin;
        pm->date = 0;

        pm->tot_kids  = 0;
        pm->kid_num   = kidnum;

        pm->child   = NULL;
        pm->parent  = NULL;
        pm->sibling = NULL;
    }
    else
    {
        BERROR("Can't alloc media entry\n");
    }

    return pm;
}

void media_delete(media_obj_t *pm)
{
    if(pm)
    {
        if(pm->id)        MEDIAFREE(pm->id);
        if(pm->title)     MEDIAFREE(pm->title);
        if(pm->subtitle)  MEDIAFREE(pm->subtitle);
        if(pm->artist)    MEDIAFREE(pm->artist);
        if(pm->album)     MEDIAFREE(pm->album);
        if(pm->url)       MEDIAFREE(pm->url);
        if(pm->iurl)      MEDIAFREE(pm->iurl);
        MEDIAFREE(pm);
    }
}

void media_sub_delete(media_tree_t *tree, media_obj_t *pm)
{
    media_obj_t *px;

    //media_treeDump(pm);
    //Log(2, "----\n");

    // if the current object ptr is in the tree below this object
    // then pass up the current to our sibling (or parent)
    //
    for(px = tree->current; px; px = px->parent)
    {
        if(px == pm)
        {
            if(pm->sibling)
            {
                px = pm->sibling;
            }
            else
            {
                px = pm->parent;
            }
            tree->current = px;
            break;
        }
    }

    if(pm == tree->root)
    {
        tree->root = pm->sibling;
    }

    // remove object from alloc chain
    //
    if(pm == tree->media)
    {
        tree->media = pm->next;
    }
    else
    {
        for(px = tree->media; px && px->next != pm;)
        {
            px = px->next;
        }

        if(px)
        {
            px->next = pm->next;
        }
    }

    if (pm->child)
    {
        media_sub_delete(tree, pm->child);
        pm->child = NULL;
    }
    if (pm->sibling)
    {
        media_sub_delete(tree, pm->sibling);
        pm->sibling = NULL;
    }
    media_delete(pm);
}

void media_delete_from_tree(media_tree_t *tree, media_obj_t *pm)
{
    media_obj_t *px;

    if (! pm)
    {
        return;
    }

    // remove media object from tree, which also
    // means all sub-objects are pruned as well
    //
    if (pm->parent)
    {
        if (pm == pm->parent->child)
        {
            pm->parent->child = pm->sibling;
        }
        else
        {
            for (px = pm->parent->child; px->sibling != pm;)
            {
                px = px->sibling;
            }
            if (px)
            {
                px->sibling = pm->sibling;
            }
        }
        pm->parent = NULL;
    }

    media_sub_delete(tree, pm);
}

void media_copy(media_obj_t *pdm, media_obj_t *pm)
{
    if(pdm->title)    MEDIAFREE(pdm->title);
    if(pdm->subtitle) MEDIAFREE(pdm->subtitle);
    if(pdm->id)       MEDIAFREE(pdm->id);
    if(pdm->url)      MEDIAFREE(pdm->url);
    if(pdm->iurl)     MEDIAFREE(pdm->iurl);

    memcpy(pdm, pm, sizeof(media_obj_t));

    if(pm->title)
    {
        pdm->title = (char*)MEDIAALLOC(strlen(pm->title) + 1);
        if(pdm->title)
        {
            strcpy(pdm->title, pm->title);
        }
    }
    if(pm->subtitle)
    {
        pdm->subtitle = (char*)MEDIAALLOC(strlen(pm->subtitle) + 1);
        if(pdm->subtitle)
        {
            strcpy(pdm->subtitle, pm->subtitle);
        }
    }
    if(pm->artist)
    {
        pdm->artist = (char*)MEDIAALLOC(strlen(pm->artist) + 1);
        if(pdm->artist)
        {
            strcpy(pdm->artist, pm->artist);
        }
    }
    if(pm->album)
    {
        pdm->album = (char*)MEDIAALLOC(strlen(pm->album) + 1);
        if(pdm->album)
        {
            strcpy(pdm->album, pm->album);
        }
    }
    if(pm->id)
    {
        pdm->id = (char*)MEDIAALLOC(strlen(pm->id) + 1);
        if(pdm->id)
        {
            strcpy(pdm->id, pm->id);
        }
    }
    if(pm->url)
    {
        pdm->url = (char*)MEDIAALLOC(strlen(pm->url) + 1);
        if(pdm->url)
        {
            strcpy(pdm->url, pm->url);
        }
    }
    if(pm->iurl)
    {
        pdm->iurl = (char*)MEDIAALLOC(strlen(pm->iurl) + 1);
        if(pdm->iurl)
        {
            strcpy(pdm->iurl, pm->iurl);
        }
    }
}

media_obj_t *media_duplicate(media_obj_t *pm)
{
    media_obj_t *pr;

    if(! pm)
    {
        return NULL;
    }

    pr = media_create(pm->id, pm->kid_num, pm->type, pm->title);
    if(pm->subtitle)
    {
        pr->subtitle = (char*)MEDIAALLOC(strlen(pm->subtitle) + 1);
        strcpy(pr->subtitle, pm->subtitle);
    }
    else
    {
        pr->subtitle = NULL;
    }

    if(pm->url)
    {
        pr->url = (char*)MEDIAALLOC(strlen(pm->url) + 1);
        strcpy(pr->url, pm->url);
    }
    else
    {
        pr->url = NULL;
    }

    if(pm->iurl)
    {
        pr->iurl = (char*)MEDIAALLOC(strlen(pm->iurl) + 1);
        strcpy(pr->iurl, pm->iurl);
    }
    else
    {
        pr->iurl = NULL;
    }

    if(pm->artist)
    {
        pr->artist = (char*)MEDIAALLOC(strlen(pm->artist) + 1);
        strcpy(pr->artist, pm->artist);
    }
    else
    {
        pr->artist = NULL;
    }

    if(pm->album)
    {
        pr->album = (char*)MEDIAALLOC(strlen(pm->album) + 1);
        strcpy(pr->album, pm->album);
    }
    else
    {
        pr->album = NULL;
    }

    pm->reliability = -1;
    pr->tot_kids    = 0;
    pr->bits    =   pm->bits;
    pr->chan    =   pm->chan;
    pr->rate    =   pm->rate;
    pr->brt     =   pm->brt;
    pr->size    =   pm->size;
    pr->type    =   pm->type;
    pr->codec   =   pm->codec;
    pr->date    =   pm->date;

    return pr;
}

media_obj_t *media_insert_in_tree(media_tree_t *tree, media_obj_t *pm, media_obj_t *ppm)
{
    media_obj_t *pxm;
    media_obj_t *pxxm;

    if(! pm)
    {
        return NULL;
    }

    if(ppm)
    {
        if(ppm->child)
        {
            for(pxxm = NULL, pxm = ppm->child; pxm; pxm = pxm->sibling)
            {
                if(pm->id && pxm->id && ! strcmp(pxm->id, pm->id))
                {
                    //Log(3, "media_ - Duplicate media entry: %s\n", pm->title);
                    media_delete_from_tree(tree, pm);
                    return NULL;
                }

                if(pm->kid_num < pxm->kid_num)
                {
                    pm->sibling = pxm;
                    if(pxm == ppm->child)
                    {
                        ppm->child = pm;
                    }
                    else
                    {
                        pxxm->sibling = pm;
                    }
                    break;
                }
                else if(! pxm->sibling)
                {
                    pxm->sibling = pm;
                    pm->sibling  = NULL;
                    break;
                }
                else
                {
                    pxxm = pxm;
                }
            }
        }
        else
        {
            ppm->child = pm;
        }
        pm->parent = ppm;
    }
    else if(tree)
    {
        if(tree->root)
        {
            for(pxxm = NULL, pxm = tree->root; pxm; pxm = pxm->sibling)
            {
                if(pm->kid_num < pxm->kid_num)
                {
                    pm->sibling = pxm;
                    if(pxm == ppm->child)
                    {
                        ppm->child = pm;
                    }
                    else
                    {
                        pxxm->sibling = pm;
                    }
                    break;
                }
                else if(! pxm->sibling)
                {
                    pxm->sibling = pm;
                    pm->sibling  = NULL;
                    break;
                }
                else
                {
                    pxxm = pxm;
                }
            }
        }
        else
        {
            tree->root = pm;
        }
    }

    if(tree)
    {
        pm->next = tree->media;
        tree->media = pm;
    }

    return pm;
}

int media_recover_memory(media_tree_t *tree, media_obj_t *pnode)
{
    media_obj_t *pm;

    if(! tree || ! pnode)
    {
        return 0;
    }

    // low-mem recovery step 1:
    //   ask the media tree owner (the application) to compact
    //   other system memory
    //

    // low-mem recovery step 2:
    //   find a sibling of parent node that has a subtree and free it
    //
    if(pnode->parent)
    {
        for(pm = pnode->parent->child; pm; pm = pm->sibling)
        {
            if(pm != pnode)
            {
                if(pm->child)
                {
                    // found sibling node which isn't us that has kids, so free it
                    // and hope that frees enough memory
                    //
                    media_delete_from_tree(tree, pm->child);
                    pm->tot_kids = 0;
                    pm->child = NULL;
                    return 1;
                }
            }
        }
    }

    // low-mem recovery step 3:
    //   delete a child of this parent which might result in incomplete listings
    //   but we are already close to comprimised, so what else is possible?
    //
    if(pnode->child)
    {
        media_delete_from_tree(tree, pnode->child);
        pnode->tot_kids = 0;
        pnode->child = NULL;
        return 1;
    }

    BERROR("Media - memory recovery exhausted\n");
    return 0;
}

media_obj_t *media_create_in_tree(const char* id, int kidnum, media_obj_type_t type, const char* name, media_tree_t *tree, media_obj_t *pparent)
{
    media_obj_t *pm;

    do
    {
        // create a media tree entry for this object
        //
        pm = media_create(id, kidnum, type, name);
        if(pm)
        {
            // insert it in the proper spot
            return media_insert_in_tree(tree, pm, pparent);
        }
    }
    while(media_recover_memory(tree, pparent));

    return NULL;
}


media_obj_t *media_get_current_object(media_tree_t *tree)
{
    if(! tree)
    {
        return NULL;
    }

    return tree->current;
}

media_obj_t *media_get_root_object(media_tree_t *tree)
{
    if(! tree)
    {
        return NULL;
    }

    return tree->root;
}

media_obj_t *media_set_current_object(media_tree_t *tree, media_obj_t *pm)
{
    if(! tree)
    {
        return NULL;
    }

    return tree->current = pm;
}

media_obj_t *media_get_obj_by_id(media_tree_t *tree, const char *id)
{
    media_obj_t *pm;

    if(! strcmp(id, "0"))
    {
        return NULL;
    }

    for(pm = tree ? tree->media : NULL; pm; pm = pm->next)
    {
        if(pm->id && ! strcmp(id, pm->id))
        {
            return pm;
        }
    }
    return NULL;
}

media_obj_t *media_get_obj_by_name(media_tree_t *tree, const char *name)
{
    media_obj_t *pm;

    for(pm = tree ? tree->media : NULL; pm; pm = pm->next)
    {
        if(pm->title && ! strcmp(name, pm->title))
        {
            return pm;
        }
    }
    return NULL;
}

int media_set_url(media_obj_t *pm, const char *url)
{
    if(! pm)
    {
        return 0;
    }

    if(pm->url)
    {
        MEDIAFREE(pm->url);
        pm->url = NULL;
    }

    if(url)
    {
        pm->url = (char*)MEDIAALLOC(strlen(url) + 1);
        if(pm->url)
        {
            strcpy(pm->url, url);
            return 1;
        }
    }
    return 0;
}

char *media_get_url(media_obj_t *pm)
{
    if(! pm)
    {
        return NULL;
    }

    return pm->url;
}

int media_set_iurl(media_obj_t *pm, const char *url)
{
    if(! pm)
    {
        return -1;
    }

    if(pm->iurl)
    {
        MEDIAFREE(pm->iurl);
        pm->iurl = NULL;
    }

    if(url)
    {
        pm->iurl = (char*)MEDIAALLOC(strlen(url) + 1);
        if(pm->iurl)
        {
            strcpy(pm->iurl, url);
            return 0;
        }
    }
    return -1;
}

char *media_get_iurl(media_obj_t *pm)
{
    if(! pm)
    {
        return NULL;
    }

    return pm->iurl;
}

int media_set_title(media_obj_t *pm, const char *title)
{
    if(! pm)
    {
        return -1;
    }

    if(pm->title)
    {
        MEDIAFREE(pm->title);
        pm->title = NULL;
    }

    if(title)
    {
        pm->title = (char*)MEDIAALLOC(strlen(title) + 1);
        if(pm->title)
        {
            strcpy(pm->title, title);
            return 0;
        }
    }
    return -1;
}

char *media_get_title(media_obj_t *pm)
{
    if(! pm)
    {
        return NULL;
    }

    return pm->title;
}

int media_set_sub_title(media_obj_t *pm, const char *subtitle)
{
    if(! pm)
    {
        return -1;
    }

    if(pm->subtitle)
    {
        MEDIAFREE(pm->subtitle);
        pm->subtitle = NULL;
    }

    if(subtitle)
    {
        pm->subtitle = (char*)MEDIAALLOC(strlen(subtitle) + 1);
        if(pm->subtitle)
        {
            strcpy(pm->subtitle, subtitle);
            return 0;
        }
    }
    return -1;
}

char *media_get_sub_title(media_obj_t *pm)
{
    if(! pm)
    {
        return NULL;
    }

    return pm->subtitle;
}

int media_set_artist(media_obj_t *pm, const char *artist)
{
    if(! pm)
    {
        return -1;
    }

    if(pm->artist)
    {
        MEDIAFREE(pm->artist);
        pm->artist = NULL;
    }

    if(artist)
    {
        pm->artist = (char*)MEDIAALLOC(strlen(artist) + 1);
        if(pm->artist)
        {
            strcpy(pm->artist, artist);
            return 0;
        }
    }
    return -1;
}

char *media_get_artist(media_obj_t *pm)
{
    if(! pm)
    {
        return NULL;
    }

    return pm->artist;
}

int media_set_album(media_obj_t *pm, const char *album)
{
    if(! pm)
    {
        return -1;
    }

    if(pm->album)
    {
        MEDIAFREE(pm->album);
        pm->album = NULL;
    }

    if(album)
    {
        pm->album = (char*)MEDIAALLOC(strlen(album) + 1);
        if(pm->album)
        {
            strcpy(pm->album, album);
            return 0;
        }
    }
    return -1;
}

char *media_get_album(media_obj_t *pm)
{
    if(! pm)
    {
        return NULL;
    }

    return pm->album;
}

int media_better_protocol(mime_content_type_t np, mime_content_type_t op)
{
#if 0
    if(g_invert_format_priorities)
    {
        return (op == mpUNKNOWN || (int)np < (int)op);
    }
    else
    {
        return (int)np > (int)op;
    }
#endif
}

void media_sort(media_obj_t *parent, media_sorter_t sorter)
{
    media_obj_t *px;
    media_obj_t *pm;
    media_obj_t *ps;
    media_obj_t *pz;
    int n;

    if(! parent || ! sorter)
    {
        return;
    }

    // head of list
    px = parent->child;
    pz = NULL;

    while(px)
    {
        for(ps = px, pm = px->sibling; pm; pm = pm->sibling)
        {
            n = sorter(ps, pm);
            if(n > 0)
            {
                ps = pm;
            }
        }
        if(! pz)
        {
            parent->child = ps;
        }
        else
        {
            pz->sibling = ps;
        }
        pz = ps;

        if(ps == px)
        {
            px = px->sibling;
        }
        else
        {
            for(pm = px; pm->sibling != ps;)
            {
                pm = pm->sibling;
            }
            pm->sibling = ps->sibling;
        }

        ps->sibling = NULL;
    }

    for(px = parent->child, n = 0; px; px = px->sibling)
    {
        px->kid_num = n++;
    }
}
