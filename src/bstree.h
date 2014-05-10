#ifndef BSTREE
#define BSTREE

#include "btree.h"

struct bstree_node
{
	struct btree_node *root;
};

struct bstree_node *bstree_create();

void bstree_insert(int value, struct bstree_node *node);

void bstree_destroy(struct bstree_node *node);

void bstree_print(struct bstree_node *node);

#endif