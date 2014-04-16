#include "score.h"
#include "pacghost.h"
#include <curses.h>
#include <sys/ioctl.h>

void display_score() {
	attrset(COLOR_PAIR(1));
	mvprintw(w.ws_row - 2, 0, "%s %d", "Score: ", score);
	mvprintw(w.ws_row - 1, 0, "%s %d", "Live:  ", live);
	mvprintw(w.ws_row - 2, w.ws_col / 4 * 3, "%s %d", "Level: ", level);
	attrset(COLOR_PAIR(3));
}

void update_score(char * map, int height, int width, struct pacghost * pacman, int * currentScore, int * atePelet ) {
	//pelet +1point
	//fruit +10
	if(map[width * pacman[0].xLocation + pacman[0].yLocation] == 's'){
		currentScore[0] = currentScore[0] + 1;
		map[width * pacman[0].xLocation + pacman[0].yLocation] = ' ';
		(*atePelet)++;
	}
	if(map[width * pacman[0].xLocation + pacman[0].yLocation] == 'f' || map[width * pacman[0].xLocation + pacman[0].yLocation] == 'F' ){
		currentScore[0] = currentScore[0] + 10;
		map[width * pacman[0].xLocation + pacman[0].yLocation] = ' ';
	}
}

int isWin(int atePelet, int totalPelet){
	if(atePelet == totalPelet){
		return 1;
	} else {
		return 0;
	}
}
