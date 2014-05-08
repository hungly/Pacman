#include "nguyenvinhlinh.h"
#include "pacghost.h"
#include <curses.h>
int face2face(struct pacghost * ghost);
void directToPacman(struct pacghost * ghost);
int findRelativePosition(struct pacghost * ghost);
int checkTopExit(struct pacghost * ghost);
int checkLeftExit(struct pacghost * ghost);
int checkRightExit(struct pacghost * ghost);
int checkBottomExit(struct pacghost * ghost);
int isSurround(struct pacghost * ghost);
int face2faceTrap(struct pacghost * ghost);
// void nguyenvinhlinh_ai(struct pacghost * ghost);

int face2face(struct pacghost * ghost){
	char map2d[height][width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map2d[i][j] = map[width*i+j];
		}
	}
	if (ghost->yLocation == pacman.yLocation){
		if(ghost->xLocation < pacman.xLocation){
			for (int i = ghost->xLocation; i < pacman.xLocation; i++) {
				//check if there is an obstacle between them
				if((map2d[ghost->yLocation][i] == 's' ||map2d[ghost->yLocation][i] == 'S' || map2d[ghost->yLocation][i] == 'f' || map2d[ghost->yLocation][i] == 'F' ||  map2d[ghost->yLocation][i] == 'p' ||  map2d[ghost->yLocation][i] == 'P') == 0 ){
					break;
					return -1;
				}
			}
			return 2;
		} else {
			for (int i = pacman.xLocation; i < ghost->xLocation; i++) {
				//check if there is an obstacle between them
				if((map2d[ghost->yLocation][i] == 's' ||map2d[ghost->yLocation][i] == 'S' || map2d[ghost->yLocation][i] == 'f' || map2d[ghost->yLocation][i] == 'F' || map2d[ghost->yLocation][i] == 'P' || map2d[ghost->yLocation][i] == 'p') == 0 ){
					break;
					return -1;
				}
			}
			return 0;
		}
	} else if (ghost->xLocation == pacman.xLocation){
		if(ghost->yLocation < pacman.yLocation){
			for (int i =  ghost->yLocation; i < pacman.yLocation; i++) {
				if((map2d[i][ghost->xLocation] == 's' || map2d[i][ghost->xLocation] == 'S' || map2d[i][ghost->xLocation] == 'f' || map2d[i][ghost->xLocation] == 'F' || map2d[i][ghost->xLocation] == 'P' || map2d[i][ghost->xLocation] == 'p') == 0){
					break;
					return -1;
				}
			}
			return 1;
		} else {
			for (int i =  pacman.yLocation; i < ghost->yLocation; i++) {
				if((map2d[i][ghost->xLocation] == 's' || map2d[i][ghost->xLocation] == 'S' || map2d[i][ghost->xLocation] == 'f' || map2d[i][ghost->xLocation] == 'F' || map2d[i][ghost->xLocation] == 'P' || map2d[i][ghost->xLocation] == 'p') == 0){
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
	if(ghost->direction == 0 && checkTopExit(ghost)== 0 && ghost->xLocation == pacman.xLocation){
		ghost->direction = 1;
	} else if (ghost->direction == 1 && checkRightExit(ghost) == 0 && ghost->xLocation == pacman.xLocation){
		ghost->direction = 0;
	} else if (ghost->direction == 2 && checkBottomExit(ghost) == 0 && ghost->yLocation == pacman.yLocation){
		ghost->direction = 3;
	} else if (ghost->direction == 3 && checkLeftExit(ghost) == 0 && ghost->xLocation == pacman.xLocation){
		ghost->direction = 0;
	} else 
	//if current direction = 0
	if(ghost->direction == 0 || ghost->direction == 2){
		if(checkLeftExit(ghost) == 1 && (findRelativePosition(ghost) == 1 || findRelativePosition(ghost) == 4)){
			ghost->direction = 3;
		}else if(checkRightExit(ghost) == 1 && (findRelativePosition(ghost) == 2 || findRelativePosition(ghost) == 3)){
			ghost->direction = 1;
		}
	} else if (ghost->direction == 1 || ghost->direction == 3){
		if(checkTopExit(ghost) == 1 && ((findRelativePosition(ghost) == 1) || findRelativePosition(ghost) == 2)){
			ghost->direction = 0;
		}else if(checkBottomExit(ghost) == 1 && (findRelativePosition(ghost) == 3 || findRelativePosition(ghost)== 4)){
			ghost->direction = 2;
		}
	}
}

int face2faceTrap(struct pacghost * ghost){
	if(ghost->yLocation == pacman.yLocation && ghost->xLocation > pacman.xLocation  && ghost->direction == 0 && checkTopExit(ghost) == 0){
		printw("f2fTrap1");
		ghost->direction = 1;
		return 1;
	} else if(ghost->yLocation == pacman.yLocation && ghost->xLocation < pacman.xLocation && ghost->direction == 2 &&checkBottomExit(ghost)==0){

		printw("f2fTrap3");
		ghost->direction = 3;
		return 1;
	} else if(ghost->xLocation == pacman.xLocation && ghost->yLocation > pacman.yLocation && ghost->direction == 3 && checkLeftExit(ghost)==0){
		printw("f2fTrap0");
		ghost->direction = 0;
		return 1;
	} else if (ghost->xLocation == pacman.xLocation && ghost->yLocation < pacman.yLocation && ghost->direction == 1 && checkRightExit(ghost)==0){
		printw("f2fTrap2");
		ghost->direction = 2;
		return 1;
	}
	return 0;
}

int findRelativePosition(struct pacghost * ghost){
	if(pacman.yLocation <= ghost->yLocation && pacman.xLocation <= ghost->xLocation)return 1;
	if(pacman.yLocation <= ghost->yLocation && pacman.xLocation >= ghost->xLocation)return 4;
	if(pacman.yLocation >= ghost->yLocation && pacman.xLocation <= ghost->xLocation)return 2;
	if(pacman.yLocation >= ghost->yLocation && pacman.xLocation >=  ghost->xLocation)return 3;
	return 0;
}

int checkLeftExit(struct pacghost * ghost){
	char map2d[height][width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map2d[i][j] = map[width*i+j];
		}
	}
	if(map2d[ghost->xLocation][ghost->yLocation-1] == 's' ||map2d[ghost->xLocation][ghost->yLocation-1] == 'S'|| map2d[ghost->xLocation][ghost->yLocation-1] == ' ' || map2d[ghost->xLocation][ghost->yLocation-1] == 'f' || map2d[ghost->xLocation][ghost->yLocation-1] == 'F'){
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
	if(map2d[ghost->xLocation][ghost->yLocation+1] == 's' || map2d[ghost->xLocation][ghost->yLocation+1] == 'S' || map2d[ghost->xLocation][ghost->xLocation+1] == 'f' || map2d[ghost->xLocation][ghost->yLocation+1] == 'F' || map2d[ghost->xLocation][ghost->yLocation+1] == ' '){
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

	if(map2d[ghost->xLocation-1][ghost->yLocation] == 's' || map2d[ghost->xLocation-1][ghost->yLocation] == 'S' || map2d[ghost->xLocation-1][ghost->yLocation] == 'f' || map2d[ghost->xLocation-1][ghost->yLocation] == 'F' || map2d[ghost->xLocation-1][ghost->yLocation] == ' '){
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
	if(map2d[ghost->xLocation+1][ghost->yLocation] == 's' || map2d[ghost->xLocation+1][ghost->yLocation] == 'S'|| map2d[ghost->xLocation+1][ghost->yLocation] == 'f'|| map2d[ghost->xLocation+1][ghost->yLocation] == 'F' || map2d[ghost->xLocation+1][ghost->yLocation] == ' '){
		return 1;
	} else {
		return 0;
	}
}
//this function will check the current situation of ghost, if it's trapped or
//not., then return possibel exit 0, 1, 2, 3 
int isSurround( struct pacghost * ghost){
	char map2d[height][width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map2d[i][j] = map[width*i+j];
		}
	}
	int number_of_surrounders = 0;
	if(checkTopExit(ghost) == 0){
		number_of_surrounders = number_of_surrounders + 1;
	}
	if(checkRightExit(ghost) == 0){
		number_of_surrounders = number_of_surrounders + 2;
	}
	if(checkBottomExit(ghost) == 0){
		number_of_surrounders = number_of_surrounders + 4;
	}
	if(checkLeftExit(ghost) == 0){
		number_of_surrounders = number_of_surrounders + 8;
	}
	
	if(number_of_surrounders == 14){
		return 0; //0
	} else if (number_of_surrounders == 13){
		return 1; // 1
	} else if (number_of_surrounders == 11){
		return 2; // 2
	} else if (number_of_surrounders == 7){
		return 3;// 3
	}else {
		return -1;
	}
	
}

void nguyenvinhlinh_ai(char * map, struct pacghost * pacman, struct pacghost * ghost, const int difficulty, const int is_pacman_powered_up){
	int trapped = 0;
	if(ghost->direction == 4){
		ghost->direction = 0;
		return;
	}

	if(is_pacman_powered_up == 1){
		if(face2face(ghost) == 1){
			if(face2face(ghost) == 0){
				ghost->direction = 1;
			} else 	if(face2face(ghost) == 1){
				ghost->direction = 3;
			} else 	if(face2face(ghost) == 2){ 
				ghost->direction = 0;
			} else 	if(face2face(ghost) == 3){
				ghost->direction = 1;
			}
		} else {
			if(ghost->direction == 0 || ghost->direction == 2){
				if(findRelativePosition(ghost) == 1 || findRelativePosition(ghost) == 4){
					if(checkRightExit(ghost) == 1){
						ghost->direction = 1;
					}
				} else if (findRelativePosition(ghost) == 2 || findRelativePosition(ghost) == 3){
					if(checkLeftExit(ghost) == 1){
						ghost->direction = 3;
					}
				}
			} else if (ghost->direction == 1 || ghost->direction == 3){
				if(findRelativePosition(ghost) == 1 || findRelativePosition(ghost) == 2){
					if(checkBottomExit(ghost) == 1){
						ghost->direction = 2;
					}
				} else if (findRelativePosition(ghost) == 3 || findRelativePosition(ghost) == 4){
					if(checkTopExit(ghost) == 1){
						ghost->direction = 0;
					}
				}
			}
		    
		}
		return;
	}
	
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
	} 
	
	else {
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
		} else if(checkTopExit(ghost) == 0 && checkLeftExit(ghost) == 0 && findRelativePosition(ghost) == 1){
			ghost->direction = 1;
		} else if(checkTopExit(ghost)== 0 && checkRightExit(ghost) == 0 && findRelativePosition(ghost) == 2){
			ghost->direction = 3;
		} else if (checkRightExit(ghost) == 0 && checkBottomExit(ghost) == 0 && findRelativePosition(ghost)==3){
			ghost->direction = 0;
		} else if (checkLeftExit(ghost) == 0 && checkBottomExit(ghost) == 0 && findRelativePosition(ghost) == 4){
			ghost->direction = 0;
		}
		else {
			directToPacman(ghost);
		}		
	}
	char map2d[height][width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map2d[i][j] = map[width*i+j];
		}
	}
	/*
	move(0, 0);
	printw("relative position: %d %d %d", findRelativePosition(ghost), height, width);
	move(1, 0);
	printw("position of ghost: %d - %d - %d", ghost->yLocation, ghost->xLocation, ghost->direction);
	move(2, 0);
	//printw("position of pacman: x%d - y%d - %c", pacman->xLocation, pacman->yLocation, map2d[pacman->xLocation][pacman->yLocation]);
	printw("checkTop: %d - checkRight: %d - checkBottom: %d - checkLeft: %d", checkTopExit(pacman), checkRightExit(pacman), checkBottomExit(pacman), checkLeftExit(pacman));
	move(3, 0);
	printw("------------------------------------");
	move(3, 0);
	printw("isSurround: %d", isSurround(ghost));
	move(4, 0);
	printw("------------------------------------");
	move(4, 0);
	printw("danger: %d", is_pacman_powered_up);
	*/
}