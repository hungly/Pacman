#include "pacghost.h"
#include "movement.h"
#include <stdio.h>
int isCollision(struct pacghost * pacman, struct pacghost * ghost){
	if(pacman->xLocation == ghost[0].xLocation && pacman->yLocation == ghost[0].yLocation ){
		return 1;
	}
	if(pacman->xLocation == ghost[1].xLocation && pacman->yLocation == ghost[1].yLocation ){
		return 1;
	}
	if(pacman->xLocation == ghost[2].xLocation && pacman->yLocation == ghost[2].yLocation ){
		return 1;
	}
	if(pacman->xLocation == ghost[3].xLocation && pacman->yLocation == ghost[3].yLocation ){
		return 1;
	}
	return 0;
}

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
