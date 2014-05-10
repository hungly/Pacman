#include <stdlib.h>
#include <stdio.h>
#include "btree.h"

struct btree_node *btree_create(int value, struct btree_node *parent)
{
	struct btree_node *node = malloc(sizeof(struct btree_node));
	if (node != NULL)
	{
		node->value = value;
		node->parent = parent;
		node->left = NULL;
		node->right = NULL;
		return node;
	}
	else
	{
		return NULL;
	}
}

void btree_destroy(struct btree_node *node)
{
	if (node != NULL)
	{
		if (node->left != NULL)
		{
			btree_destroy(node->left);
		}
		if (node->right != NULL)
		{
			btree_destroy(node->right);
		}
		free(node);
	}
}

void btree_print(struct btree_node *node)
{
	if (node == NULL)
	{

	}
	else
	{
		printf("Value: %d", node->value);
		if (node->parent != NULL)
		{
			printf("\tParent: %d", node->parent->value);
		}
		if (node->left != NULL)
		{
			printf("\tLeft: %d", node->left->value);
		}
		if (node->right != NULL)
		{
			printf("\tRight: %d", node->right->value);
		}
		btree_print(node->left);
		btree_print(node->right);
	}
}