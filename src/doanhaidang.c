#include <stdlib.h>
#include "level_editor.h"
#include "pacghost.h"
#include "cell.h"
#include "queue.h"
#include "btree.h"
#include "bstree.h"
#include "doanhaidang.h"

struct bstree_node *cell_value_tree;

int *cell_value_array;

struct cell *cell_array;

int count = 0;

struct queue *queue;

char *ai_map;

int is_valid_cell(int x, int y)
{
	switch (map[x + y * width])
	{
		case ' ':
		case 'o':
		case 's':
		case 'S':
		case 'f':
		case 'F':
		case 'g':
		case 'G':
		case 'p':
		case 'P':
			return 0;
		default:
			return 1;
	}
}

void add_cell(int x, int y, char direction)
{
	switch (ai_map[y * width + x])
	{
		case ' ':
		case 's':
		case 'S':
		case 'f':
		case 'F':
		case 'g':
		case 'G':
		case 'p':
		case 'P':
			bstree_insert(x + y * width, cell_value_tree);
			cell_value_array[count] = x + y * width;
			count += 1;
			ai_map[y * width + x] = 'o';
			break;
		default:
			return;
	}
	if (is_valid_cell(x + 1, y) == 0 && direction != 'l') add_cell(x + 1, y, 'r');
	if (is_valid_cell(x - 1, y) == 0 && direction != 'r') add_cell(x - 1, y, 'l');
	if (is_valid_cell(x, y + 1) == 0 && direction != 'd') add_cell(x, y + 1, 'u');
	if (is_valid_cell(x, y - 1) == 0 && direction != 'u') add_cell(x, y - 1, 'd');
}

void collect_valid_cell(int pacman_x, int pacman_y)
{
	cell_value_array = malloc(sizeof(int) * width * height);
	cell_value_tree = bstree_create();
	add_cell(pacman_x, pacman_y, ' ');
}

void init_dang_ghost(int pacman_x, int pacman_y)
{
	ai_map = malloc(sizeof(char) * (width * height));
	for (int i = 0; i < width * height; i++) ai_map[i] = map[i];
	collect_valid_cell(pacman_x, pacman_y);
	cell_array = malloc(sizeof(struct cell) * count);
	for (int j = 0; j < count; j++)
	{
		cell_array[j].value = cell_value_array[j];
		for (int k = 0; k < 4; k++) cell_array[j].adjs[k] = -1;
		int curval = cell_array[j].value;
		int beta_adjs[4] = {-1, -1, -1, -1};
		if (0 <= curval && curval <= width - 1)
		{
			if (curval == 0)
			{
				beta_adjs[0] = curval + 1;
				beta_adjs[1] = curval + width;
			}
			else if (curval == width - 1)
			{
				beta_adjs[0] = curval - 1;
				beta_adjs[1] = curval + width;
			}
			else
			{
				beta_adjs[0] = curval - 1;
				beta_adjs[1] = curval + 1;
				beta_adjs[2] = curval + width;
			}
		}
		else if (height * width - width <= curval && curval <= height * width - 1)
		{
			if (curval == height * width - width)
			{
				beta_adjs[0] = curval + 1;
				beta_adjs[1] = curval - width;
			}
			else if (curval == height * width - 1)
			{
				beta_adjs[0] = curval - 1;
				beta_adjs[1] = curval - width;
			}
			else
			{
				beta_adjs[0] = curval - 1;
				beta_adjs[1] = curval + 1;
				beta_adjs[2] = curval - width;
			}
		}
		else
		{
			if (curval % width == 0)
			{
				beta_adjs[0] = curval + 1;
				beta_adjs[1] = curval - width;
				beta_adjs[2] = curval + width;
			}
			else if (curval % width == width - 1)
			{
				beta_adjs[0] = curval - 1;
				beta_adjs[1] = curval - width;
				beta_adjs[2] = curval + width;
			}
			else
			{
				beta_adjs[0] = curval - 1;
				beta_adjs[1] = curval + 1;
				beta_adjs[2] = curval - width;
				beta_adjs[3] = curval + width;
			}
		}
		for (int k = 0; k < 4; k++)
		{
			char is_found = 'n';
			if (beta_adjs[k] == -1) continue;
			else
			{
				struct btree_node *curnode = cell_value_tree->root;
				while (curnode != NULL)
				{
					int compval = curnode->value;
					if (beta_adjs[k] == compval)
					{
						is_found = 'y';
						break;
					}
					else if (beta_adjs[k] < compval)
					{
						if (curnode->left != NULL) curnode = curnode->left;
						else
						{
							is_found = 'n';
							break;
						}
					}
					else
					{
						if (curnode->right != NULL) curnode = curnode->right;
						else
						{
							is_found = 'n';
							break;
						}
					}
				}
				if (is_found == 'n') beta_adjs[k] = -1;
			}
		}
		for (int l = 0; l < 4; l++) cell_array[j].adjs[l] = beta_adjs[l];
	}
}

void bfs(int x, int y)
{
	int map_i = x + y * width;
	for (int i = 0; i < count; i++)
	{
		if (cell_array[i].value == map_i) queue = queue_create(&cell_array[i]);
		cell_array[i].color = 'w';
	}
	while (queue->size > 0)
	{
		struct cell *curcell = dequeue(&queue);
		if (curcell != NULL)
		{
			for (int i = 0; i < 4; i++)
			{
				if (curcell->adjs[i] != -1)
				{
					for (int j = 0; j < count; j++)
					{
						if (cell_array[j].value == curcell->adjs[i])
						{
							if (cell_array[j].color == 'w')
							{
								cell_array[j].color = 'g';
								cell_array[j].pred = curcell->value;
								enqueue(&cell_array[j], &queue);
							}
							break;
						}
					}
				}
			}
		}
	}
}

void move_dang_ghost(char * map,struct pacghost *pacman, struct pacghost *ghost,const int difficulty, int powered_up)
{
	bfs(pacman->yLocation, pacman->xLocation);
	int map_i = ghost->yLocation + ghost->xLocation * width;
	for (int i = 0; i < count; i++)
	{
		if (cell_array[i].value == map_i)
		{
			if (cell_array[i].pred == map_i - 1)
			{
				if (powered_up == 0) ghost->direction = 3;
				else ghost->direction = 1;
				break;
			}
			else if (cell_array[i].pred == map_i + 1)
			{
				if (powered_up == 0) ghost->direction = 1;
				else ghost->direction = 1;
				break;
			}
			else if (cell_array[i].pred == map_i - width)
			{
				if (powered_up == 0) ghost->direction = 0;
				else ghost->direction = 1;
				break;
			}
			else if (cell_array[i].pred == map_i + width)
			{
				if (powered_up == 0) ghost->direction = 2;
				else ghost->direction = 1;
				break;
			}
		}
	}
}

void finish_dang_ghost()
{
	if (cell_value_tree != NULL) bstree_destroy(cell_value_tree);
	if (cell_value_array != NULL) free(cell_value_array);
	if (cell_array != NULL) free(cell_array);
	if (queue != NULL) queue_destroy(queue);
	if (ai_map != NULL) free(ai_map);
}
