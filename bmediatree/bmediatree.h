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
#ifndef BMEDIATREE_H
#define BMEDIATREE_H 1

#include "bnetheaders.h"
#include "butil.h"

#define MEDIA_MAX_TITLE	(256)

// ----------------------------------------------------------------------
typedef enum
{
	mtFOLDER			= 0x04,
	mtVOLATILEFOLDER	= 0x06,
	mtWEBFOLDER			= 0x05,
	mtVOLATILEWEBFOLDER	= 0x07,
	mtSTATIONFOLDER		= 0x44,

	mtSONG				= 0x08,
	mtVIDEO				= 0x09,
	mtPLAYLIST			= 0x18,

	mtSTREAMSONG		= 0x28,
	mtSTREAMPLAYLIST	= 0x38,
	mtRDTIMPLAYLIST		= 0x68,

	mtFMRADIO			= 0x40,
	mtAMRADIO			= 0x41,
}
media_obj_type_t;

#define mtVOLATILE		0x02
#define mtFOLDER_TYPE	0x04
#define mtSONG_TYPE		0x08
#define mtPLAYLIST_TYPE	0x10
#define mtSTREAM		0x20
#define mtRADIO_TYPE	0x40
#define mtLOOP			0x80

// this struct is the dominant data structure of the media player
// and represents one node in a tree of songs (leaves) and folders (nodes)
// under each media server.  The tree is discarded only when the server
// it belongs to indicates it has changed its tree.  This "shadow" of
// the server's tree is technically not required, but it greatly improves
// responsiveness to the user, and decouples networks latency.
//
// be careful about the size of the node, since an iPod could have
// perhaps 10,000 songs.  For 10K songs, average title and url length
// of 64 bytes, each node is about 128 bytes.  so 10K is 1.2 MB for the tree
// and, if ALL the songs are placed in a play-list, that is an additional
// 1/2 mb about, since the playlist references the tree, so count on having
// about 2mb ram for the max media tree
//
// the structure allows for "holes" in sibling lists so that partial results
// from servers can be used for scrolling very large song lists.  The hole
// is formed by knowing how many total siblings are needed to fill a list
// (total kids) and having each kid know what its canonical place in the
// server list it represents
//
// ----------------------------------------------------------------------
typedef struct media_obj
{
	char               *title;
	char               *subtitle;
	char               *artist;
	char               *album;
	char               *id;
	char               *url;
	char               *iurl;
	int                 reliability;

	int                 tot_kids;	// total siblings expected in child
	int                 kid_num;	// which sibling I am, in server coordinates

	int                 rate;
	int                 brt;
	int                 size;
	unsigned char       bits;
	unsigned char       chan;
	media_obj_type_t 	type;
	mime_content_type_t codec;
	time_t				date;

	struct media_obj   *parent;
	struct media_obj   *sibling;
	struct media_obj   *child;
	struct media_obj   *next;
}
media_obj_t;

typedef struct media_tree
{
	media_obj_t *media;		///< linear list of media
	media_obj_t *root;		///< root of tree of media organized hierarchically
	media_obj_t *current;	///< current "cursor" position in tree
}
media_tree_t;

typedef int (*media_sorter_t)(media_obj_t *a, media_obj_t *b);

media_tree_t *media_tree_create		(void);
void		media_tree_destroy		(media_tree_t *tree);
void 		media_tree_dump			(media_tree_t *tree, media_obj_t *pm);

media_obj_t *media_create			(const char *id, int kidnum, media_obj_type_t type, const char *name);
media_obj_t *media_create_in_tree	(const char *id, int kidnum, media_obj_type_t type, const char *name, media_tree_t *tree, media_obj_t *parent);
void		media_delete			(media_obj_t *pm);
void		media_delete_from_tree	(media_tree_t *tree, media_obj_t *pm);
media_obj_t *media_duplicate		(media_obj_t *pm);
void		media_copy				(media_obj_t *pdstm, media_obj_t *psrcm);
media_obj_t *media_insert_in_tree	(media_tree_t *tree, media_obj_t *pm, media_obj_t *parent);

int			media_recover_memory	(media_tree_t *tree, media_obj_t *pnode);

media_obj_t *media_get_by_id		(media_tree_t *tree, const char *id);
media_obj_t *media_get_by_name		(media_tree_t *tree, const char *name);

media_obj_t *media_get_current_obj	(media_tree_t *tree);
media_obj_t *media_set_current_obj	(media_tree_t *tree, media_obj_t *pm);
media_obj_t *media_get_root_obj		(media_tree_t *tree);

int			media_setURL			(media_obj_t *pm, const char *url);
char		*media_getURL			(media_obj_t *pm);
int			media_setIURL			(media_obj_t *pm, const char *url);
char		*media_getIURL			(media_obj_t *pm);
int			media_better_protocol	(mime_content_type_t newp, mime_content_type_t oldp);

int			media_set_title			(media_obj_t *pm, const char *title);
char		*media_get_title		(media_obj_t *pm);
int			media_set_subtitle		(media_obj_t *pm, const char *subtitle);
char		*media_get_subtitle		(media_obj_t *pm);
int			media_set_artist		(media_obj_t *pm, const char *artist);
char		*media_get_artist		(media_obj_t *pm);
int			media_set_album			(media_obj_t *pm, const char *album);
char		*media_get_album		(media_obj_t *pm);

void		media_sort				(media_obj_t * pParent, media_sorter_t sorter);

#endif

