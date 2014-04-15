#include "level_editor.h"
#include "score.h"
#include "core.h"
#include "pacghost.h"
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <regex.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

/** Define the directory when opening using sh file in root folder of project*/
#define DIRECTORY_SH            "levels/"

/** Define the directory when opening using the executable file in source folder*/
#define DIRECTORY_SRC           "../levels/"

/** Define ColorPallette1 for terminal background */
#define COLOR_BACKGROUND        0

/** Define ColorPallette2 for map background */
#define COLOR_MAP_BACKGROUND    1

/** Define ColorPallette3 for error message */
#define COLOR_ERROR_BACKGROUND  2

/** Define ColorPallette4 for letters */
#define COLOR_LETTER            3

/** Define ColorPallette5 for wall in map */
#define COLOR_WALL              4

/** Define ColorPallette6 for pacman */
#define COLOR_PACMAN            5

/** Define ColorPallette7 for fruit */
#define COLOR_FRUIT             6

/** Define ColorPallette8 for ghost */
#define COLOR_GHOST             7

/** Pointer to array of char which indicates the current map */
char *map;

/** Pointer to array to char which indicates the author of current map */
char *author;

/** Pointer to array to char which indicates the title of current map */
char *title;

/** Pointer to array to char which indicates the file name of current map */
char *file_name;

/** Pointer to array to char which indicates the present working directory */
char *directory;

/** The size of ncruses terminal in characters of height and width */
struct winsize w;

/** An integer value which indicate the program is ended or not */
int end_program = 0;

/** An integer value which indicate the number of error message */
int error_msg_count = 1;

/** An integer value which indicates the height of map */
int height;

/** An integer value which indicates the width of map */
int width;

/** An integer value which indicates the row where the cursor is located in the map */
int x = 0;

/** An integer value which indicates the column where the cursor is located in the map */
int y = 0;

/** An integer value which indicates the distance (in rows) between the top of terminal and the first row of the map */
int x_offset;

/** An integer value which indicates the distance (in columns) between the left most of terminal and the first column of the map */
int y_offset;

/** regex is used to  check for input pattern */
regex_t regex;

/** reti is an integer which store the result of regex comparision */
int reti;

int score = 0;

int live = 0;

int level = 1;

struct pacghost pacman;
struct pacghost ghosts[4];
struct pacghost * ghost = &(ghosts[0]);

int main(int argc, char *argv[]) {
	int input;
	int count = 0;

	struct timespec delay = {0, 500000000L}, 
                     rem;

	/* determine the current working directory and the levels folder for save map */
    if (startsWith("./src/", argv[0])) {
        directory = DIRECTORY_SH;
    } else {
        directory = DIRECTORY_SRC;
    }
    
    /* initialise ncurses screen */
    initscr();
    timeout(20);
    noecho();

    /* set color pairs */
    if(has_colors()) {
        start_color();
        init_pair(1, COLOR_LETTER,  COLOR_BACKGROUND);
        init_pair(2, COLOR_LETTER,  COLOR_ERROR_BACKGROUND);
        init_pair(3, COLOR_WALL,    COLOR_MAP_BACKGROUND);
        init_pair(4, COLOR_PACMAN,  COLOR_MAP_BACKGROUND);
        init_pair(5, COLOR_FRUIT,   COLOR_MAP_BACKGROUND);
        init_pair(6, COLOR_LETTER,  COLOR_MAP_BACKGROUND);
        init_pair(7, COLOR_GHOST,	COLOR_MAP_BACKGROUND);
        init_pair(8, COLOR_PACMAN,	COLOR_BACKGROUND);
        init_pair(9, COLOR_GHOST,	COLOR_BACKGROUND);
    }

    read_file("level3.pac");

    /* get terminal size */
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    do {
    	mvprintw(w.ws_row - 1, w.ws_col - 10, "%d", count);

    	display_map(map);

    	display_score();

    	search_pacman(map, &pacman);
	    search_ghost(map, ghost);

    	display_characters(&pacman, ghost);

    	input = getch();

    	count++;

        nanosleep(&delay, &rem);

    } while (input != 'q' || input != 'Q');

    free(map);

    endwin();
    exit(0);
}