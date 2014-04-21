#include "core.h"
#include "level_editor.h"
#include "movement.h"
#include "pacghost.h"
#include "score.h"
#include <curses.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
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
/** Pointer to array to char which indicates the author of current map */
char *author;
/** Pointer to array to char which indicates the present working directory */
char *directory;
/** Pointer to array of char which indicates the list of availble levels */
char * levels[] = {"level2.pac", "level3.pac", "level1.pac"};
/** Pointer to array to char which indicates the file name of current map */
char *file_name;
/** Pointer to array of char which indicates the current map */
char *map;
/** Pointer to array to char which indicates the title of current map */
char *title;
int atePellet = 0;
int end_game = 0;
/** An integer value which indicate the program is ended or not */
int end_program = 0;
/** An integer value which indicate the number of error message */
int error_msg_count = 1;
/** An integer value which indicate the number of maps availble */
int num_levels = 3;
int live = 3;
int level = 1;
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
/** reti is an integer which store the result of regex comparision */
int reti;
int score = 0;
int totalPellet;
/** regex is used to  check for input pattern */
regex_t regex;
struct pacghost pacman;
struct pacghost ghosts[4];
struct pacghost * ghost = &(ghosts[0]);
/** The size of ncruses terminal in characters of height and width */
struct winsize w;
void showMenu(int argc, char *argv[]);
void startNewGame(int argc, char *argv[]);
int main(int argc, char *argv[]) {
	initscr();
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
	showMenu(argc, argv);
    endwin();
    exit(0);
}

void showMenu(int argc, char *argv[]){
	while(1){
		clear();
		move(0,0);
		printw("1. Start new game\n2. Edit mode\n3. High score\n4. Game credit\n5. Quit\n");
		printw("Press 1,2,3,4 to start function, other to quit program\n");
		//	timeout(-1);
		int choice = getch();
		if(choice == '1'){
			startNewGame(argc, argv);
		} else if (choice == '2'){
			editor(argc, argv);
		} else if (choice == '3'){
			//High score
			printw("high score");
		} else if(choice == '4'){
			printw("Game credit");
		} else {
			endwin();
			return;
		}
	}
}

void startNewGame(int argc, char *argv[]){
	int input;
	int count = 0;
	struct timespec delay = {0, 250000000L}, 
                     rem;
	/* determine the current working directory and the levels folder for save map */
    if (startsWith("./src/", argv[0])) {
        directory = DIRECTORY_SH;
    } else {
        directory = DIRECTORY_SRC;
    }
    /* enable the use of function keys, allow navigating the cursor using arrow keys */
    keypad(stdscr, TRUE);
    /* disable echo when getch */
    noecho();
    /* take input chars, does not wait until new line or carriage return */
    cbreak();
    timeout(0);
    /* get terminal size */
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    while(!end_game) {
    	mvprintw(w.ws_row - 1, w.ws_col - 1, "%d", levels[(level - 1) % num_levels]);
    	read_file(levels[(level - 1) % num_levels]);

    	display_map(map);

    	attrset(COLOR_PAIR(9));
    	mvprintw(w.ws_row - 2, 0, "Score: ");
		mvprintw(w.ws_row - 1, 0, "Live:  ");
		mvprintw(w.ws_row - 2, w.ws_col / 4 * 3 + 7, "Level: ");
		attrset(COLOR_PAIR(1));
    	search_pacman(map, &pacman);
    	search_ghost(map, ghost);
    	atePellet = 0;
    	count_pellet(map, &totalPellet);
    	pacman.direction = 4;
    	for (int i = 0; i < 4; i++) {
    		ghost[i].direction = 4;
    	}
    	// if neccessary call initalise functions for ghost here
	    while (!end_game) {
	    	input = getch();
	        if (input == 'q' || input == 'Q')
	        {
	            end_game = 1;
	        } else {
	            switch (input) {
	                case KEY_UP:
	                    if (isValidMoveCell(pacman.xLocation - 1, pacman.yLocation)) {
	                        pacman.direction = 0;
	                    } else if (canMove(&pacman)) {
	                        ungetch(input);
	                    }
	                    break;
	                case KEY_RIGHT:
	                    if (isValidMoveCell(pacman.xLocation, pacman.yLocation + 1)) {
	                        pacman.direction = 1;
	                    } else if (canMove(&pacman)) {
	                        ungetch(input);
	                    }
	                    break;
	                case KEY_DOWN:
	                    if (isValidMoveCell(pacman.xLocation + 1, pacman.yLocation)) {
	                        pacman.direction = 2;
	                    } else if (canMove(&pacman)) {
	                        ungetch(input);
	                    }
	                    break;
	                case KEY_LEFT:
	                    if (isValidMoveCell(pacman.xLocation, pacman.yLocation - 1)) {
	                        pacman.direction = 3;
	                    } else if (canMove(&pacman)) {
	                        ungetch(input);
	                    }
	                    break;
	                default:
	                    break;
	            }
	        }
	        if (isWin(atePellet, totalPellet)) {
	            mvprintw (0, 0, "%d,%d,%d", atePellet, totalPellet, level);
	            level++;
	            //timeout(-1);
	            //getch();
	            break;
	        }
	        update_score(map, height, width, &pacman, &score, &atePellet);
	        // the two following lines is for debuging
	        mvprintw(w.ws_row - 1, w.ws_col - 10, "%d", count);
	        mvprintw(w.ws_row - 1, w.ws_col - 30, "%5d,%5d", atePellet, totalPellet);
	        delete_characters(&pacman, ghost);
	        move_character(&pacman);
	        // call move function for the ghosts
	        for (int i = 0; i < 4; i++) {
	        	move_character(&ghosts[i]);
	        }
	        for (int i = 0; i < 4; i++) {
	            if (isCollision(&pacman, &ghosts[i])) {
	                if (live <= 1) {
	                    end_game = 1;
	                    break;
	                } else {
	                    live--;
	                    search_pacman(map, &pacman);
	                }
	            }
	        }
	        display_score();
	        display_characters(&pacman, ghost);  
	        nanosleep(&delay, &rem);
	    }
	    // if neccessary call clean up functions for ghost here
	}
	if (map) {
		free(map);
	}
	if (author) {
		free(author);
	}
    if (title) {
    	free(title);
    }
    if (file_name) {
    	free(file_name);
    }
}
