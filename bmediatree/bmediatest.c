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
#include "bmediatree.h"

static int sort_alpha_title(media_obj_t *a, media_obj_t *b)
{
	if (! a || ! b)
	{
		return 0;
	}

	if (! a->title)
	{
		if (! b->title)
		{
			return 0;
		}

		return 1;
	}
	else
	{
		if (! b->title)
		{
			return -1;
		}

		return strcmp(a->title, b->title);
	}
}

int main(int argc, char **argv)
{
	media_tree_t *tree;
	media_obj_t *root;
	media_obj_t *obj;
	media_obj_t *dir;
	media_obj_t *child;

	butil_set_log_level(5);

	tree = media_tree_create();
	if (! tree)
	{
		BERROR("Can't make tree");
		return -1;
	}

	// create root dir item
	root = media_create_in_tree("0", 0, mtFOLDER, "root", tree, NULL);
	tree->root = tree->current = root;

	// create top level dir music
	obj = media_create_in_tree("00A", 0, mtFOLDER, "Music", tree, root);

	child = media_create_in_tree("M1", 0, mtSONG, "asong1", tree, obj);
	child = media_create_in_tree("M2", 1, mtSONG, "bsong2", tree, obj);
	child = media_create_in_tree("M3", 2, mtSONG, "csong3", tree, obj);

	// create top level dir videos
	obj = media_create_in_tree("00B", 0, mtFOLDER, "Videos", tree, root);

	dir = media_create_in_tree("0BA", 0, mtFOLDER, "Good", tree, obj);

	child = media_create_in_tree("GV1", 0, mtVIDEO, "cvid1", tree, dir);
	child = media_create_in_tree("GV2", 1, mtVIDEO, "bvid2", tree, dir);
	child = media_create_in_tree("GV3", 2, mtVIDEO, "avid3", tree, dir);

	dir = media_create_in_tree("0BB", 1, mtFOLDER, "Bad", tree, obj);

	child = media_create_in_tree("BV1", 0, mtVIDEO, "cvid1", tree, dir);
	child = media_create_in_tree("BV2", 1, mtVIDEO, "bvid2", tree, dir);
	child = media_create_in_tree("BV3", 2, mtVIDEO, "avid3", tree, dir);

	butil_log(2, "Base Tree:\n");
	media_tree_dump(tree, root);

	media_sort(dir, sort_alpha_title);

	butil_log(2, "Sorted by title Tree:\n");
	media_tree_dump(tree, root);

	return 0;
}

