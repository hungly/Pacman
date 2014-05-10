#include "hung.h"
#include "movement.h"
#include "pacghost.h"
#include "level_editor.h"
#include <stdlib.h>
#include <curses.h>
#include <sys/ioctl.h>

int * ai_map;
int * ai_map_height;
int * ai_map_width;
int ** nodes_cost;
int num_nodes;
int pacman_spawn_point_x;
int pacman_spawn_point_y;

void initialise_hung_ai(const char * map, const int height, const int width) {
	timeout(-1);
	ai_map_width = malloc(sizeof(int));
	*ai_map_width = width;
	ai_map_height = malloc(sizeof(int));
	*ai_map_height = height;

	ai_map = malloc(sizeof(int) * (*ai_map_height * *ai_map_width));

	for (int i = 0; i < *ai_map_height * *ai_map_width; i++) {
		switch (map[i]) {
			case ' ':
			case 's':
			case 'S':
			case 'f':
			case 'F':
			case 'g':
			case 'G':
			case 'p':
			case 'P':
				ai_map[i] = -3;
				break;
			default:
				ai_map[i] = -2;
				break;
		}
	}

	search_pacman_spawn_point(&pacman_spawn_point_x, &pacman_spawn_point_y);
	auto_find_path(pacman_spawn_point_x, pacman_spawn_point_y, ' ');

	num_nodes = 0;

	for (int i = 0; i < *ai_map_height * *ai_map_width; i++) {
		if (ai_map[i] == -1) {
			switch (count_available_move_cell(i / *ai_map_width, i % *ai_map_width)) {
				case 2:
					if (is_corner_cell(i / *ai_map_width, i % *ai_map_width)) {
						ai_map[i] = num_nodes;
						num_nodes++;
					}
					break;
				default:
					ai_map[i] = num_nodes;
					num_nodes++;
					break;
			}
		}
	}

	for (int i = 0; i < *ai_map_height * *ai_map_width; i++) {

	}

	nodes_cost = malloc(sizeof(int *) * num_nodes);

	for (int i = 0; i < num_nodes; i++) {
		nodes_cost[i] = malloc(sizeof(int) * num_nodes);
	}

	for (int i = 0; i < num_nodes; i++) {
		for (int j = 0; j < num_nodes; j++) {
			nodes_cost[i][j] = 0;
		}
	}

	for (int i = 0; i < *ai_map_height * *ai_map_width; i++) {
		if (ai_map[i] >= 0) {
			for (int j = 0; j < 4; j++) {
			}
		}
	}

	display_ai_map();
	getch();
	display_cost_table();
	getch();
}

int count_available_move_cell(const int x, const int y) {
	int count = 0;

	if ((x - 1) >= 0 && isValidMoveCell(x - 1, y)) {
		count++;
	}
	if ((x + 1) < *ai_map_height && isValidMoveCell(x + 1, y)) {
		count++;
	}
	if ((y - 1) >= 0 && isValidMoveCell(x, y - 1)) {
		count++;
	}
	if ((y + 1) < *ai_map_width && isValidMoveCell(x, y + 1)) {
		count++;
	}
	return count;
}

void display_ai_map() {
	/* calculate the offset to display the map on screen */
    int x_offset = (((w.ws_row - 6) / 2) - (*ai_map_height / 2)) > 0 ? ((w.ws_row -6) / 2) - (*ai_map_height / 2): 0;
    int y_offset = ((w.ws_col / 2) - (*ai_map_width / 2)) > 0 ? (w.ws_col / 2) - (*ai_map_width / 2): 0;

    /* display character base on character stored in memory */
    for (int i = 0; i < *ai_map_height * *ai_map_width; i++) {

        if (i != 0 && i % *ai_map_width == 0) {
            move(4 + x_offset + (i / *ai_map_width), y_offset);
        }

        //if (ai_map[i] != -1) {
        mvprintw(4 + x_offset + (i / *ai_map_width), y_offset + (i % *ai_map_width), "%d", ai_map[i]);
        //}
    }
}

void display_cost_table() {
	move(0,0);

	for (int i = 0; i < num_nodes; i++) {
		for (int j = 0; j < num_nodes; j++) {
			printw("%d", nodes_cost[i][j]);
		}
		move(i + 1, 0);
	}
}

void finish_hung_ai() {
	if(ai_map) {
		free(ai_map);
	}
	if (nodes_cost) {
		for (int i = 0; i < num_nodes; i++) {
			if (nodes_cost[i]) {
				free(nodes_cost[i]);
			}
		}
		free(nodes_cost);
	}
	if (ai_map_width) {
		free(ai_map_width);
	}
	if (ai_map_height) {
		free(ai_map_height);
	}
}

int is_corner_cell(const int x, const int y) {
	if ((x - 1) >= 0 && isValidMoveCell(x - 1, y) && (x + 1) < *ai_map_height && isValidMoveCell(x + 1, y)) {
		return 0;
	} else if ((y - 1) >= 0 && isValidMoveCell(x, y - 1) && (y + 1) < *ai_map_width && isValidMoveCell(x, y + 1)) {
		return 0;
	} else {
		return 1;
	}
}

void auto_find_path(const int x, const int y, const char direction) {

	switch (ai_map[(x) * width + y]) {
		case -3:
			ai_map[(x) * width + y] = -1;
			break;
		default:
			return;
	}

	/* recursively call itself in other cell after fill it with 's' if it contains ' ' */

	if (isValidFillCell(x + 1, y) && direction != 'u') {
		auto_find_path(x + 1, y, 'd');
	}
	if (isValidFillCell(x, y + 1) && direction != 'l') {
		auto_find_path(x, y + 1, 'r');
	}
	if (isValidFillCell(x - 1, y) && direction != 'd') {
		auto_find_path(x - 1, y, 'u');
	}
	if (isValidFillCell(x, y - 1) && direction != 'r') {
		auto_find_path(x, y - 1, 'l');
	}
}

void calculate_cost(const int i, int * end, int * cost, const int j) {
}