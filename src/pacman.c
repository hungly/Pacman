#include "core.h"
#include "level_editor.h"
#include "movement.h"
#include "pacghost.h"
#include "score.h"
#include "lyquochung.h"
#include "nguyenvinhlinh.h"
#include "doanhaidang.h"
#include "lyquanliem.h"
#include <curses.h>
#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
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
char * levels[] = {"level1.pac", "level2.pac", "level3.pac", "level4.pac"};

/** Pointer to array to char which indicates the file name of current map */
char *file_name;

/** Pointer to array of char which indicates the current map */
char *map;

/** Pointer to array to char which indicates the title of current map */
char *title;

/** An double array which indicates the speed changing rate of the game based on difficulty */
double speed_change_rate[] = {0.99, 0.97, 0.95, 0.93};

/** An integer value which indicates the number of pellet that been eaten */
int atePellet = 0;

/** An integer value which indicates the game difficulty */
int difficulty = 3;

/** An integer value which indicates the time left for the pacman powered up */
int counter = 0;

/** An integer value which indicates the game is over or not */
int end_game = 0;

/** An integer value which indicate the program is ended or not */
int end_program = 0;

/** An integer value which indicate the number of error message */
int error_msg_count = 1;

/** An integer value which indicate the number of maps availble */
int ghost_counter[] = {0, 0, 0, 0};

/** An integer value which indicates the number of life left for pacman */
int live = 3;

/** An integer value which indicates the current level of the player */
int level = 1;

/** An integer value which indicates level offset on the list */
int level_offset = 0;

/** An integer value which indicates the number of level for before circle arround */
int num_levels = 4;

/** An integer value which indicates the powered up status of the pacman */
int is_pacman_powered_up = 0;

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

/** An integer value which indicates the current score */
int score = 0;

/** An integer value which indicates the total number of pellet in the map */
int totalPellet;

/** An integer value which indicates the time for each game 'frame' */
long turns_time;

/** regex is used to  check for input pattern */
regex_t regex;

struct pacghost pacman;

struct pacghost ghosts[4];

struct pacghost * ghost = &(ghosts[0]);

/** The size of ncruses terminal in characters of height and width */
struct winsize w;

/** Funtion pointers for AI */
void (*ai_functions[4])(char * map, struct pacghost * pacman, struct pacghost * ghost, int difficulty, int is_pacman_powered_up);
/**
  * Show the game's main menu
  */
void showMenu(int argc, char *argv[]);

/**
  * Start the game.
  */
void startNewGame(int argc, char *argv[]);
void change_difficulty();

int main(int argc, char *argv[]) {
	/* initiallse function name */
	ai_functions[0] = hung_ai;
	ai_functions[1] = nguyenvinhlinh_ai;
	ai_functions[2] = choose_direction_for_ghost;
	ai_functions[3] = move_dang_ghost;

	// ai_functions[3] = dang's AI here;
	initscr();

    /* get terminal size */
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	/* method are given from http://en.chys.info/2009/09/esdelay-ncurses/ */
    if (getenv ("ESCDELAY") == NULL)
        ESCDELAY = 25;

    /* set color pairs */
    if(has_colors()) {
        start_color();
        init_pair(1, COLOR_LETTER, COLOR_BACKGROUND);
        init_pair(2, COLOR_LETTER, COLOR_ERROR_BACKGROUND);
        init_pair(3, COLOR_WALL,   COLOR_MAP_BACKGROUND);
        init_pair(4, COLOR_PACMAN, COLOR_MAP_BACKGROUND);
        init_pair(5, COLOR_FRUIT,  COLOR_MAP_BACKGROUND);
        init_pair(6, COLOR_LETTER, COLOR_MAP_BACKGROUND);
        init_pair(7, COLOR_GHOST,  COLOR_MAP_BACKGROUND);
        init_pair(8, COLOR_PACMAN, COLOR_BACKGROUND);
        init_pair(9, COLOR_GHOST,  COLOR_BACKGROUND);
        init_pair(10, COLOR_FRUIT,  COLOR_BACKGROUND);
    }
    // show the main menu
	showMenu(argc, argv);

	// finsh the program
    endwin();
    exit(0);
}

void showMenu(int argc, char *argv[]) {
	while (1) {
		/* get terminal size */
    	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		y_offset = w.ws_col / 2;
		// hide the cursor
		curs_set(0);
		attrset(COLOR_PAIR(9));
		end_game = 0;
		clear();
		// tittle
		mvprintw(8, y_offset - 19, " _____        _____ __  __          _   _ ");
		mvprintw(9, y_offset - 19, "|  __ \\ /\\   / ____|  \\/  |   /\\   | \\ | |");
		mvprintw(10, y_offset - 19, "| |__) /  \\ | |    | \\  / |  /  \\  |  \\| |");
		mvprintw(11, y_offset - 19, "|  ___/ /\\ \\| |    | |\\/| | / /\\ \\ | . ` |");
		mvprintw(12, y_offset - 19, "| |  / ____ \\ |____| |  | |/ ____ \\| |\\  |");
		mvprintw(13, y_offset - 19, "|_| /_/    \\_\\_____|_|  |_/_/    \\_\\_| \\_|");
		// menu
		mvprintw(20, y_offset - 8, "1. Start new game");
		mvprintw(21, y_offset - 8, "2. Edit mode");
		mvprintw(22, y_offset - 8, "3. High score");
		mvprintw(23, y_offset - 8, "4. Game credit");
		mvprintw(24, y_offset - 8, "5. Quit");
		mvprintw(w.ws_row - 2, y_offset - 20, "Press 1, 2, 3, 4 or 5 to start function");
		timeout(-1);
		// get the choice
		int choice = getch();
		if (choice == '1'){
			// start a new game
			clear();
			change_difficulty();
			startNewGame(argc, argv);
		} else if (choice == '2'){
			// start level editor
			clear();
			editor(argc, argv);
		} else if (choice == '3'){
			// display high score list
			clear();
			display_high_score(argc, argv);
		} else if(choice == '4') {
			// display game credit
			credit_game();
		} else if(choice == '5'){
			// exit the program
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
			return;
		}
	}
}

void startNewGame(int argc, char *argv[]){
	int input;
	timeout(0);
	// rset game status
	counter = 0;
	level_offset = 0;
	level = 1;
	live = 3;
	score = 0;
	atePellet = 0;
	struct timespec delay = {0, 250000000L}, rem, start, end;
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
    while(!end_game) {
    	// update turn time based on level and diffivultly
    	turns_time = 200000000L * pow(speed_change_rate[difficulty], level / num_levels);
    	// reset counter
    	counter = 0;
    	for (int i = 0; i < 4; i++) {
    		ghost_counter[i] = 0;
    	}
    	// load the map from the list
    	read_file(levels[(level - 1 + level_offset) % num_levels]);

    	// display the map on the screen
    	display_map(map);

    	attrset(COLOR_PAIR(9));
    	// spawn the pacman on it spawn point
    	if (!search_pacman(map, &pacman)) {
	    	timeout(-1);
	    	clear();
	    	// no ghost's spawn point found
	    	mvprintw(0, 0, "No pacman spawn points found on \"%s\" map\nDo you want to load the next map in the queue?\nYes (y)/No (n)", levels[(level - 1 + level_offset) % num_levels]);
	    	int temp_input = getch();
	    	while(temp_input != 'y' && temp_input != 'n' && temp_input != 'Y' && temp_input != 'N') {
	    		mvprintw (3, 0, "Invalid input! Please try again!");
	    		temp_input = getch();
	    	}
	    	if (temp_input == 'y' || temp_input == 'Y') {
	    		level_offset++;
	    		continue;
	    	} else {
	    		end_game = 1;
	    		break;
	    	}
	    }
		pacman.direction = 4;

    	// check for ghost spawn point
	    if (!search_ghost(map, &ghosts[0], 0)) {
	    	timeout(-1);
	    	clear();
	    	// no ghost's spawn point found
	    	mvprintw(0, 0, "No ghost spawn points found on \"%s\" map\nDo you want to load the next map in the queue?\nYes (y)/No (n)", levels[(level - 1 + level_offset) % num_levels]);
	    	int temp_input = getch();
	    	while(temp_input != 'y' && temp_input != 'n' && temp_input != 'Y' && temp_input != 'N') {
	    		mvprintw (3, 0, "Invalid input! Please try again!");
	    		temp_input = getch();
	    	}
	    	if (temp_input == 'y' || temp_input == 'Y') {
	    		level_offset++;
	    		continue;
	    	} else {
	    		end_game = 1;
	    		break;
	    	}
	    }

    	// search for ghost spawn point
    	for (int i = 0; i < 4; i++) {
    		search_ghost(map, &ghosts[i], i);
	    }

		for (int i = 0; i < 4; i++)
			ghost[i].direction = 4;

	    // initilise ate pellet
		atePellet = 0;
		// count the total number of pelet
		count_pellet(map, &totalPellet);

		// initialise ai (if needed)
		initialise_hung_ai();
		initialise_ghost_map();
		init_dang_ghost(pacman);

		// display game status label
		attrset(COLOR_PAIR(9));
		mvprintw(w.ws_row - 2, 0, "Score: ");
		mvprintw(w.ws_row - 1, 0, "Live:  ");
		mvprintw(w.ws_row - 2, w.ws_col / 4 * 3 + 7, "Level: ");
		attrset(COLOR_PAIR(1));

		// start the game loop
		while (!end_game) {
		   	clock_gettime(CLOCK_REALTIME, &start);

	    	timeout(0);
		    delay.tv_nsec = turns_time;
		    // key input
		    input = getch();
		    switch (input) {
	            case KEY_UP:
	            	// move up
		            if (isValidMoveCell(pacman.xLocation - 1, pacman.yLocation)) 
		                pacman.direction = 0;
		            else if (canMove(&pacman))
		                ungetch(input);
		            break;
		        case KEY_RIGHT:
		        	// move right
		            if (isValidMoveCell(pacman.xLocation, pacman.yLocation + 1)) {
	                    pacman.direction = 1;
	                } else if (canMove(&pacman)) {
	                    ungetch(input);
		            }
		            break;
		        case KEY_DOWN:
		        	// move down
		            if (isValidMoveCell(pacman.xLocation + 1, pacman.yLocation)) {
		                pacman.direction = 2;
		            } else if (canMove(&pacman)) {
		                ungetch(input);
		            }
		            break;
		        case KEY_LEFT:
		        	// move left
		            if (isValidMoveCell(pacman.xLocation, pacman.yLocation - 1)) {
		                pacman.direction = 3;
		            } else if (canMove(&pacman)) {
		                ungetch(input);
		            }
		            break;
	            case 27:
	            	// escape key
	            	clear();
	            	if (handle_pause_menu(&is_pacman_powered_up, &counter, &live)){
	            		end_game = 1;
	            	}
	            	break;
	            default:
	            	// none of the above
	                break;
	        }
	    	if (end_game)
		        break;

		    // check if the level is over or not
		    if (isWin(atePellet, totalPellet)) {
		        level++;
		        break;
		    }

		    // update score
		    update_score(map, height, width, &pacman, &score, &atePellet, &is_pacman_powered_up, &counter);

		    // bonus live a 10000 point
	      	if (score == 10000) 
	            live++;
	        // pacman is powered up
		    if (is_pacman_powered_up && counter == 0)
		        	is_pacman_powered_up = 0;

		    // clear old character
		    delete_characters(&pacman, ghost);

		    // for each ghost
		    for (int i = 0; i < 4; i++) {
		    	// check collision
		        if (isCollision(&pacman, &ghosts[i])) {
		        	// if collide deteced
		            if (is_pacman_powered_up) {
		            	// pacman is powered up
		               	if (ghost_counter[i] == 0) {
		               		// move the ghost back to it starting point
		                	search_ghost(map, &ghosts[i], i);
		                	// set cool down time for ghost
		                	ghost_counter[i] = 50;
		                	// give score to the player
		                	score+=200;
		                }
		            } else {
		            	// 
		                ghost[0].direction = 4;
			            if (live <= 1) {
			            	// no live left
			            	end_game_dialog();
			                end_game = 1;
			                break;
			            } else {
			            	// redue live and move pacman back to it spawn point
			                live--;
			                search_pacman(map, &pacman);
			                // search for ghost spawn point
    						for (int i = 0; i < 4; i++) {
    							search_ghost(map, &ghosts[i], i);
	    					}
			                break;
			            }
			        }
		        }
		    }

		    if (end_game)
		    	break;

		    // move the ghost if speed counter is 0, 2 or 4
	    	// move function for pacman
	    	move_character(&pacman);
	        //
	        // call AI main functions here, it should be 4 function calls after this, each for 1 ghost
	        // AI funtion should have this signature:
	        // function(map, &pacman, &ghosts[i], difficulty, is_pacman_powered_up)
	        // - map: the current map, it is a 1D array
	        // - &pacman: the pacghost struct which store coordinations and direction, the direction value are:
	        //   + 0: up
	        //   + 1: right
	        //   + 2: down
	        //   + 3: left
	        //   + 4: stay/do not move
	        // - &ghost[i] is the same as &pacman except it is an array of 4 ghost
	        // - difficulty: game difficult level (0 is easy to 3 is insane, 4 in total)
	        // - is_pacman_powered_up: pacman current status which indicate it can kill a ghost (1 is can hunt ghots and 0 is cannot)
	        //
	        // move ghost if speed counter is 0 or 3
		    for (int i = 0; i < 4; i++)
		       	if (ghost_counter[i] == 0)
			       	// call AI's function 
					(*ai_functions[i])(map, &pacman, &ghosts[i], difficulty, is_pacman_powered_up);
							
		    // call move function for the ghosts
		    for (int i = 0; i < 4; i++) 
		       	if (ghost_counter[i] == 0)
		        	move_character(&ghosts[i]);
		    
		    // display new score
		    display_score();
		    // redraw character at it new position
		    display_characters(&pacman, ghost, is_pacman_powered_up, counter, ghost_counter);
		   	
		    count_down(&counter, ghost_counter);
		    // clean input queue
		    int current_input = getch();
		    int previous_input = -1;
		    while(1) {
		    	if (current_input == -1) {
		       		ungetch(previous_input);
		       		break;
		       	} else {
		       		previous_input = current_input;
		       		current_input = getch();
		       	}
		    }

		    clock_gettime(CLOCK_REALTIME, &end);
		    delay.tv_nsec -= (end.tv_nsec - start.tv_nsec);
		    nanosleep(&delay, &rem);
		}

		// clean up AI's data
		finish_hung_ai();
		free_ghost_map();
		finish_dang_ghost();
	}
}

void change_difficulty(){
	int diff;
	do{	clear();
	printw("Enter game difficulty:\n");
	printw("1 - Easy\n");
	printw("2 - Normal\n");
	printw("3 - Hard\n");
	printw("4 - Insance\n");
	diff = getch();
	} while (diff != '1' && diff != '2' && diff != '3' && diff != '4');
	difficulty = diff - '1';

}
