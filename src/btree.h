#ifndef BTREE
#define BTREE

struct btree_node
{
	int value;
	struct btree_node *parent;
	struct btree_node *left;
	struct btree_node *right;
};

struct btree_node *btree_create(int value, struct btree_node *parent);

void btree_destroy(struct btree_node *node);

void btree_print(struct btree_node *node);

#endif