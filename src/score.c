#include "score.h"
#include "pacghost.h"
#include <curses.h>
#include <sys/ioctl.h>

void display_score() {
	mvprintw(w.ws_row - 2, 0, "%s %d", "Score: ", score);
	mvprintw(w.ws_row - 1, 0, "%s %d", "Live:  ", live);
	mvprintw(w.ws_row - 2, w.ws_col / 4 * 3, "%s %d", "Level: ", level);
}