#include "lyquanliem.h"
#include "movement.h"
#include "core.h"
#include "astar.h"
#include "level_editor.h"
#include "pacghost.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses.h>
#include <sys/ioctl.h>

void initialise_ghost_map(){
	ghost_map = malloc(sizeof(char) * (width*height));
	for (int rows = 0; rows < height; rows++){
		for (int cols = 0; cols < width; cols++){
			if (isValidMoveCell(rows,cols)){
                int count = 0;
                if (isValidMoveCell(rows+1,cols))
                    count++;
                if (isValidMoveCell(rows,cols+1))
                    count++;
                if (isValidMoveCell(rows-1,cols))
                    count++;
                if (isValidMoveCell(rows,cols-1))
                    count++;
				ghost_map[rows * width + cols] = (char)(((int)'0')+count);
			} else {
				ghost_map[rows * width + cols] = ' ';
			}
		}
	}
}

void choose_direction_for_ghost(char * map, struct pacghost * pacman, struct pacghost * ghost, int difficulty, int is_pacman_powered_up){
    if (!is_pacman_powered_up){
        if (difficulty == 0){
            move_randomly(ghost);
        } else if (difficulty == 1){
            int random = random_in_range(0,10);
            if (random < 5)
                move_randomly(ghost);
            else
                chase_after_pacman(pacman, ghost);
        } else if (difficulty == 2){
            int random = random_in_range(0,10);
            if (random < 2)
                move_randomly(ghost);
            else
                chase_after_pacman(pacman, ghost);

        } else {
            chase_after_pacman(pacman, ghost);
        }
    /* The ghost move to the point nearer to him than pacman when pacman powered */
    } else {
        /* As the point search in order. The ghost tend to stuck when came to top left */
        for (int rows = 0; rows < height; rows++) {
            for (int cols = 0; cols < width; cols++){
                if (isValidMoveCell(rows, cols)){
                    if (distance_between_two_points(pacman->xLocation,pacman->yLocation,rows,cols) > distance_between_two_points(ghost->xLocation,ghost->yLocation,rows,cols)){
                        ghost->direction = direction_from_node_to_node(ghost->xLocation,ghost->yLocation,rows,cols);
                        return;
                    }
                }
            }
        }
    }   
}

void move_randomly(struct pacghost * ghost) {
    int availableWay = ghost_map[ghost->xLocation * width + ghost->yLocation]  - '0';
    int count = 0;
    if (availableWay >= 3 || !(canMove(ghost))) {
        int random = random_in_range(0,availableWay);
        if (isValidMoveCell(ghost->xLocation-1,ghost->yLocation)){
            if (count != random){
                count++;
            } else {
                ghost->direction = 0;
                count++;
            }
        }
        if (isValidMoveCell(ghost->xLocation,ghost->yLocation+1)){
            if (count != random){
                count++;
            } else {
                ghost->direction = 1;
                count++;
            }
        }
        if (isValidMoveCell(ghost->xLocation+1,ghost->yLocation)){
            if (count != random){
                count++;
            } else {
                ghost->direction = 2;
                count++;
            }
        }
        if (isValidMoveCell(ghost->xLocation,ghost->yLocation-1)){
            if (count != random)
                count++;
            else 
                ghost->direction = 3;
        }
    }
}

void chase_after_pacman(struct pacghost * pacman, struct pacghost * ghost){
    if ((ghost_map[ghost->xLocation * width + ghost->yLocation] - '0') == 1){
        if (isValidMoveCell(ghost->xLocation-1,ghost->yLocation))
            ghost->direction = 0;        
        else if (isValidMoveCell(ghost->xLocation,ghost->yLocation+1))
            ghost->direction = 1;
        else if (isValidMoveCell(ghost->xLocation+1,ghost->yLocation))
            ghost->direction = 2;
        else if (isValidMoveCell(ghost->xLocation,ghost->yLocation-1))
            ghost->direction = 3;
    } else {
        ghost->direction = direction_from_node_to_node(ghost->xLocation,ghost->yLocation,pacman->xLocation,pacman->yLocation);
    }
}

int direction_from_node_to_node(int xStart, int yStart, int xDestination, int yDestination){
    struct list * open_list = list_create();
    struct list * close_list = list_create();
    struct node * new_node = node_create();
    new_node->x = xStart;
    new_node->y = yStart;
    new_node->g = 0;
    new_node->h = distance_between_two_points(xStart,yStart,xDestination,yDestination);
    new_node->f = new_node->h;
    open_list->root = new_node;
    while (open_list->root != NULL){
        struct node * q = node_with_least_f(open_list);
        pop_a_node_off_list(open_list,q);
        int availableWay = ghost_map[q->x * width + q->y] - '0';
        int up_added = 0;
        int right_added = 0;
        int down_added = 0;
        struct node * sucessor[availableWay];
        for (int i = 0; i < availableWay; i++){
            sucessor[i] = node_create();
            sucessor[i]->parent = q;
            if (isValidMoveCell(q->x-1,q->y) && (up_added == 0)) {
                sucessor[i]->x = q->x-1;
                sucessor[i]->y = q->y;
                up_added = 1;
            } else if (isValidMoveCell(q->x, q->y+1) && (right_added == 0)){
                sucessor[i]->x = q->x;
                sucessor[i]->y = q->y+1;
                right_added = 1;
            } else if (isValidMoveCell(q->x+1, q->y) && (down_added == 0)){
                sucessor[i]->x = q->x+1;        
                sucessor[i]->y = q->y;
                down_added = 1;
            } else if (isValidMoveCell(q->x, q->y-1)){
                sucessor[i]->x = q->x;
                sucessor[i]->y = q->y-1;      
            }
            if (sucessor[i]->x == xDestination && sucessor[i]->y == yDestination) {
                struct node * start_node = starting_node(sucessor[i]);
                if (start_node->x < xStart){
                    free_node_and_list(sucessor[i],q,open_list,close_list);
                    return 0;
                } else if (start_node->x > xStart){
                    free_node_and_list(sucessor[i],q,open_list,close_list);
                    return 2;
                } else if (start_node->y > yStart){
                    free_node_and_list(sucessor[i],q,open_list,close_list);
                    return 1;
                } else {
                    free_node_and_list(sucessor[i],q,open_list,close_list);
                    return 3;
                }
            } else {
                sucessor[i]->g = q->g + distance_between_two_points(q->x, q->y, sucessor[i]->x, sucessor[i]->y);
                sucessor[i]->h = distance_between_two_points(sucessor[i]->x,sucessor[i]->y, xDestination, yDestination);
                sucessor[i]->f = sucessor[i]->g + sucessor[i]->h;
                if ((ghost_map[sucessor[i]->x * width + sucessor[i]->y] - '0') == 1)
                    add_a_node_to_list(close_list,sucessor[i]);
                else if (better_node_exist(open_list,sucessor[i]))
                    free(sucessor[i]);
                else if (better_node_exist(close_list,sucessor[i])) 
                    free(sucessor[i]);
                else 
                    add_a_node_to_list(open_list,sucessor[i]);
            }
        }
        add_a_node_to_list(close_list, q);
    }
    return 4;
}

float sqr(float a){
    float b = a*a;
    return b;
}

float distance_between_two_points(int xStart, int yStart, int xDestination, int yDestination){
     return (float)(abs(xStart - xDestination) + abs(yStart - yDestination));
}

void free_ghost_map(){
    if (ghost_map)
        free(ghost_map);
}

int random_in_range (unsigned int min, unsigned int max){
    int base_random = rand(); /* in [0, RAND_MAX] */
    if (RAND_MAX == base_random) return random_in_range(min, max);
    /* now guaranteed to be in [0, RAND_MAX) */
    int range       = max - min,
        remainder   = RAND_MAX % range,
        bucket      = RAND_MAX / range;
    /* There are range buckets, plus one smaller interval within remainder of RAND_MAX */
    if (base_random < RAND_MAX - remainder) 
        return min + base_random/bucket;
    else
        return random_in_range (min, max);
}