#include "nguyenvinhlinh.h"
#include "pacghost.h"
int face2face(struct pacghost * ghost);
void directToPacman(struct pacghost * ghost);
int findRelativePosition(struct pacghost * ghost);
int checkTopExit(struct pacghost * ghost);
int checkLeftExit(struct pacghost * ghost);
int checkRightExit(struct pacghost * ghost);
int checkBottomExit(struct pacghost * ghost);
int isSurround(struct pacghost * ghost);
void nguyenvinhlinh_ai(struct pacghost * ghost);

int face2face(struct pacghost * ghost){
	char map2d[height][width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map2d[i][j] = map[width*i+j];
		}
	}	
	if (ghost->xLocation == pacman.xLocation){
		if(ghost->yLocation < pacman.yLocation){
			for (int i = ghost->yLocation; i < pacman.yLocation; i++) {
				//check if there is an obstacle between them
				if((map2d[ghost->xLocation][i] == 's' ||map2d[ghost->xLocation][i] == 'S' || map2d[ghost->xLocation][i] == 'f' || map2d[ghost->xLocation][i] == 'F' ||  map2d[ghost->xLocation][i] == 'p' ||  map2d[ghost->xLocation][i] == 'P') == 0 ){
					break;
					return -1;
				}
			}
			return 2;
		} else {
			for (int i = pacman.yLocation; i < ghost->yLocation; i++) {
				//check if there is an obstacle between them
				if((map2d[ghost->xLocation][i] == 's' ||map2d[ghost->xLocation][i] == 'S' || map2d[ghost->xLocation][i] == 'f' || map2d[ghost->xLocation][i] == 'F' || map2d[ghost->xLocation][i] == 'P' || map2d[ghost->xLocation][i] == 'p') == 0 ){
					break;
					return -1;
				}
			}
			return 0;
		}
	} else if (ghost->yLocation == pacman.yLocation){
		if(ghost->xLocation < pacman.xLocation){
			for (int i =  ghost->xLocation; i < pacman.xLocation; i++) {
				if((map2d[i][ghost->yLocation] == 's' || map2d[i][ghost->yLocation] == 'S' || map2d[i][ghost->yLocation] == 'f' || map2d[i][ghost->yLocation] == 'F' || map2d[i][ghost->yLocation] == 'P' || map2d[i][ghost->yLocation] == 'p') == 0){
					break;
					return -1;
				}
			}
			return 1;
		} else {
			for (int i =  pacman.xLocation; i < ghost->xLocation; i++) {
				if((map2d[i][ghost->yLocation] == 's' || map2d[i][ghost->yLocation] == 'S' || map2d[i][ghost->yLocation] == 'f' || map2d[i][ghost->yLocation] == 'F' || map2d[i][ghost->yLocation] == 'P' || map2d[i][ghost->yLocation] == 'p') == 0){
					break;
					return -1;
				}
			}
			return 3;
		}
	} else {
		return -1;
	}
}
void directToPacman(struct pacghost * ghost){
	char map2d[height][width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map2d[i][j] = map[width*i+j];
		}
	}
	//if current direction = 0
	if(ghost->direction == 0 || ghost->direction == 2){
		if(checkLeftExit(ghost) == 1 && (findRelativePosition(ghost) == 1 || findRelativePosition(ghost) == 4)){
			ghost->direction = 3;
		}else if(checkRightExit(ghost) == 1 && (findRelativePosition(ghost) == 2 || findRelativePosition(ghost) == 3)){
			ghost->direction = 1;
		}
		//check left exit
		//check right exit
	} else if (ghost->direction == 1 || ghost->direction == 3){
		if(checkTopExit(ghost) == 1 && ((findRelativePosition(ghost) == 1) || findRelativePosition(ghost) == 2)){
			ghost->direction = 0;
		}else if(checkBottomExit(ghost) == 1 && (findRelativePosition(ghost) == 3 || findRelativePosition(ghost)== 4)){
			ghost->direction = 2;
		}
	}
}

int findRelativePosition(struct pacghost * ghost){
	if(pacman.xLocation < ghost->xLocation && pacman.yLocation < ghost->yLocation)return 1;
	if(pacman.xLocation < ghost->xLocation && pacman.yLocation > ghost->yLocation)return 4;
	if(pacman.xLocation > ghost->xLocation && pacman.yLocation < ghost->yLocation)return 2;
	if(pacman.xLocation > ghost->xLocation && pacman.yLocation > ghost->yLocation)return 3;
	return 0;
}

int checkLeftExit(struct pacghost * ghost){
	char map2d[height][width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map2d[i][j] = map[width*i+j];
		}
	}
	if(map2d[ghost->xLocation-1][ghost->yLocation] == 's' ||map2d[ghost->xLocation-1][ghost->yLocation] == 'S'|| map2d[ghost->xLocation-1][ghost->yLocation] == ' ' || map2d[ghost->xLocation-1][ghost->yLocation] == 'f' || map2d[ghost->xLocation-1][ghost->yLocation] == 'F'){
		return 1;
	} else {
		return 0;
	}
}
int checkRightExit(struct pacghost * ghost){
	char map2d[height][width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map2d[i][j] = map[width*i+j];
		}
	}
	if(map2d[ghost->xLocation+1][ghost->yLocation] == 's' || map2d[ghost->xLocation+1][ghost->yLocation] == 'S' || map2d[ghost->xLocation+1][ghost->yLocation] == 'f' || map2d[ghost->xLocation+1][ghost->yLocation] == 'F' || map2d[ghost->xLocation+1][ghost->yLocation] == ' '){
		return 1;
	} else {
		return 0;
	}
}
int checkTopExit(struct pacghost * ghost) {
	char map2d[height][width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map2d[i][j] = map[width*i+j];
		}
	}

	if(map2d[ghost->xLocation][ghost->yLocation-1] == 's' || map2d[ghost->xLocation][ghost->yLocation-1] == 'S' || map2d[ghost->xLocation][ghost->yLocation-1] == 'f' || map2d[ghost->xLocation][ghost->yLocation-1] == 'F' || map2d[ghost->xLocation][ghost->yLocation-1] == ' '){
		return 1;
	} else {
		return 0;
	}
}
int checkBottomExit(struct pacghost * ghost){
	char map2d[height][width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map2d[i][j] = map[width*i+j];
		}
	}
	if(map2d[ghost->xLocation][ghost->yLocation+1] == 's' || map2d[ghost->xLocation][ghost->yLocation+1] == 'S'|| map2d[ghost->xLocation][ghost->yLocation+1] == 'f'|| map2d[ghost->xLocation][ghost->yLocation+1] == 'F' || map2d[ghost->xLocation][ghost->yLocation+1] == ' '){
		return 1;
	} else {
		return 0;
	}
}
//this function will check the current situation of ghost, if it's trapped or
//not., then return possibel exit 0,1,2,3 
int isSurround( struct pacghost * ghost){
	char map2d[height][width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map2d[i][j] = map[width*i+j];
		}
	}
	
	int number_of_surrounders = 0;
	if(map2d[ghost->xLocation][ghost->yLocation-1] == 's' || map2d[ghost->xLocation][ghost->yLocation-1] == 'S' || map2d[ghost->xLocation][ghost->yLocation-1] == 'f' || map2d[ghost->xLocation][ghost->yLocation-1] == 'F' || map2d[ghost->xLocation][ghost->yLocation-1] == ' '){
		number_of_surrounders = number_of_surrounders + 1;
	}

	if(map2d[ghost->xLocation+1][ghost->yLocation] == 's' || map2d[ghost->xLocation+1][ghost->yLocation] == 'S' || map2d[ghost->xLocation+1][ghost->yLocation] == 'f' || map2d[ghost->xLocation+1][ghost->yLocation] == 'F' || map2d[ghost->xLocation+1][ghost->yLocation] == ' '){
		number_of_surrounders = number_of_surrounders + 2;
	}
	if(map2d[ghost->xLocation][ghost->yLocation+1] == 's' || map2d[ghost->xLocation][ghost->yLocation+1] == 'S' || map2d[ghost->xLocation][ghost->yLocation+1] == 'f' || map2d[ghost->xLocation][ghost->yLocation+1] == 'F' || map2d[ghost->xLocation][ghost->yLocation+1] == ' '){
		number_of_surrounders = number_of_surrounders + 4;
	}
	if(map2d[ghost->xLocation+1][ghost->yLocation] == 's' || map2d[ghost->xLocation-1][ghost->yLocation] == 'S' || map2d[ghost->xLocation-1][ghost->yLocation] == 'f' || map2d[ghost->xLocation-1][ghost->yLocation] == 'F' || map2d[ghost->xLocation-1][ghost->yLocation] == ' '){
		number_of_surrounders = number_of_surrounders + 8;
	}

	if(number_of_surrounders == 14){
		return 0;
	} else if (number_of_surrounders == 13){
		return 1;
	} else if (number_of_surrounders == 11){
		return 2;
	} else if (number_of_surrounders == 7){
		return 3;
	}else {
		return -1;
	}
}
void nguyenvinhlinh_ai(struct pacghost * ghost){
	int trapped = 0;
	if(face2face(ghost) != -1){
		if(face2face(ghost) == 0){
			ghost->direction = 0;
		} else 	if(face2face(ghost) == 1){
			ghost->direction = 1;
		} else 	if(face2face(ghost) == 2){
			ghost->direction = 2;
		} else 	if(face2face(ghost) == 3){
			ghost->direction = 3;
		}
	} else {
		//to check that the current situation of ghost
		if(isSurround(ghost) != -1){
			trapped = 1;
			ghost->direction = isSurround(ghost);
		}
		if(trapped == 1){
			//This time, it will check that is there any way out compare to its
			//current direction, if there is an exist option direction for
			//ghost, ghost will change its current direction to new direction

			if(ghost->direction == 0 || ghost->direction == 2){
				//check left and right exit
				if(checkLeftExit(ghost) == 1){
					ghost->direction = 3;
					trapped = 0;
				} else if (checkRightExit(ghost) == 1){
					ghost->direction = 1;
					trapped = 0;
				}
			}else if (ghost->direction == 1 || ghost->direction == 3){
				//check top and bottom exit
				if(checkTopExit(ghost) == 1){
					ghost->direction = 0;
					trapped = 0;
				} else if(checkBottomExit(ghost) == 1){
					ghost->direction = 2;
					trapped = 0;
				}
			}
		} else {
			directToPacman(ghost);
		}		
	}
}
