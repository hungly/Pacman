#include "pacghost.h"
#include "movement.h"
#include <stdio.h>
#include <curses.h>

int isCollision(struct pacghost * pacman, struct pacghost * ghost){
	if (pacman->xLocation == ghost->xLocation && pacman->yLocation == ghost->yLocation) {
		return 1;
	} else if (ghost->direction != 4){
		{
			if(pacman->xLocation == ghost->xLocation && 
				(pacman->yLocation - ghost->yLocation == 0 || pacman->yLocation - ghost->yLocation == 1 || pacman->yLocation - ghost->yLocation == -1) && 
				(pacman->direction - ghost->direction == 2 || pacman->direction - ghost->direction == -2)) {
				return 1;
			}
			if(pacman->yLocation == ghost->yLocation && 
				(pacman->xLocation - ghost->xLocation == 0 || pacman->xLocation - ghost->xLocation == 1 || pacman->xLocation - ghost->xLocation == -1) && 
				(pacman->direction - ghost->direction == 2 || pacman->direction - ghost->direction == -2)) {
				return 1;
			}
		}
	}
	return 0;
}

/*
void main(){
	struct pacghost pacman;
	pacman.xLocation = 1;
	pacman.yLocation = 2;
	struct pacghost ghost[4];
	ghost[0].xLocation = 11;
	ghost[1].xLocation = 3;
	ghost[2].xLocation = 1;
	ghost[3].xLocation = 3;
	
	ghost[0].yLocation = 2;
	ghost[1].yLocation = 3;
	ghost[2].yLocation = 2;
	ghost[3].yLocation = 3;

	if(isCollision(&pacman, ghost) == 1){
		printf("true");
	}else {
		printf("false");
	}
	
}
*/

void move_character(struct pacghost * character) {
	if (character->direction == 0 && isValidMoveCell(character->xLocation - 1, character->yLocation)) {
		character->xLocation -= 1;
		return;
	}
	if (character->direction == 1 && isValidMoveCell(character->xLocation, character->yLocation + 1)) {
		character->yLocation += 1;
		return;
	}
	if (character->direction == 2 && isValidMoveCell(character->xLocation + 1, character->yLocation)) {
		character->xLocation += 1;
		return;
	}
	if (character->direction == 3 && isValidMoveCell(character->xLocation, character->yLocation - 1)) {
		character->yLocation -= 1;
		return;
	}
	if (character->direction == 4) {
		return;
	}
}

int isValidMoveCell(const int current_x, const int current_y) {
	switch (map[(current_x) * width + current_y]) {
		case ' ':
		case 's':
		case 'S':
		case 'f':
		case 'F':
		case 'g':
		case 'G':
		case 'p':
		case 'P':
			return 1;
		default:
			return 0;
	}
}

int canMove(const struct pacghost * character) {
	switch (character->direction) {
		case 0:
			if (isValidMoveCell(character->xLocation - 1, character->yLocation)) {
				return 1;
			}
			break;
		case 1:
			if (isValidMoveCell(character->xLocation, character->yLocation + 1)) {
				return 1;
			}
			break;
		case 2:
			if (isValidMoveCell(character->xLocation + 1, character->yLocation)) {
				return 1;
			}
			break;
		case 3:
			if (isValidMoveCell(character->xLocation, character->yLocation - 1)) {
				return 1;
			}
			break;
	}
	return 0;
}