#include <stdlib.h>
#include <stdio.h>
#include "bstree.h"

struct bstree_node *bstree_create()
{
	struct bstree_node *node = malloc(sizeof(struct bstree_node));
	if (node != NULL)
	{
		node->root = NULL;
		return node;
	}
	else
	{
		return NULL;
	}
}

void bstree_insert(int value, struct bstree_node *node)
{
	if (node != NULL)
	{
		struct btree_node *current;
		if (node->root == NULL)
		{
			node->root = btree_create(value, NULL);
		}
		else
		{
			current = node->root;
			while (current != NULL)
			{
				if (value < current->value)
				{
					if (current->left == NULL)
					{
						current->left = btree_create(value, current);
						break;
					}
					else
					{
						current = current->left;
					}
				}
				else
				{
					if (current->right == NULL)
					{
						current->right = btree_create(value, current);
						break;
					}
					else
					{
						current = current->right;
					}
				}
			}
		}
	}
}

void bstree_destroy(struct bstree_node *node)
{
	if (node != NULL)
	{
		btree_destroy(node->root);
		free(node);
	}
}

void bstree_print(struct bstree_node *node)
{
	if (node != NULL)
	{
		btree_print(node->root);
	}
}