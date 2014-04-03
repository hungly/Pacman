#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <regex.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

/** Define default height of map */
#define DEFAULT_HEIGHT          10

/** Define default width of the map */
#define DEFAULT_WIDTH           30

/** Define the directory when opening using sh file in root folder of project*/
#define DIRECTORY_SH            "levels/"

/** Define the directory when opening using the executable file in source folder*/
#define DIRECTORY_SRC           "../levels/"

/** Define the default author */
#define DEFAULT_AUTHOR          "Anonymous"

/** Define the default title */
#define DEAFULT_TITTLE          "Unknown"

/** Define the default file name */
#define DEAFULT_FILE_NAME       "level.pac"

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

/**
  * VI liked command mode for map pacman editor.
  * <p>This function will check if the input command is either q, w, n, r or wq and performed the following actions:</p>
  * <p>q: quit the program and exit to the terminal.</p>
  * <p>w: save the current map to the levels directory, this command accepts a parameter as the file name, if there is no file name, the map will be saved using the default name "level.pac".</p>
  * <p>wq: save and then quit the program, this command perform the same actions as the "w" command and then the "q" command.</p>
  * <p>n: create a new map, this command take three parameters, the first parameter is the new map name, the second and third parameters specify the height and width of the new map respectively.</p>
  * <p>r: load a map from the levels directory into the map editor, this command accept one parameter specify the file name of the map.</p>
  */
void command_mode();

/** 
  * Display current loaded map from an array in the heap memory.
  *
  * @param map the pointer to the array of character using to represent the current loaded map.
  */
void display_map(char *map);

/**
  * Handling input that change the current edited map.
  *
  * @param input the ASCII code of the input command.
  */
void edit_mode(int input);

/**
  * Handling new map creation using with n command in command mode and when the program is loaded.
  *
  * @param args the pointer to the array of characters which includes all three parameters which are new file name, height, and width.
  */
void new_map(char *args);

/**
  * Writing the current map and related information into a file.

  * @param file the pointer to the array of characters which indicates the file name to write into the disk.
  */
void write_file(char *file);

/**
  * Reading the current map and related information into a file.
  *
  * @param file the pointer to the array of characters which indicates the file name to read from the disk.
  */
void read_file(char *file);

/**
  * Create a new map with desire height and width.
  *
  * @param height is the height of map.
  * @param width is the width of map.
  *
  * @return pointer to an array of characters which represent the map .
  */
char *create_map(int height, int width);

/**
  * Function for checking the beginning of the string
  *
  * @param pre is the pointer to an array of char which we want to check whether the string begin with that or not.
  * @param str is the pointer to an array of char which is the string we want to check.
  *
  * @return 0 if false and other numbers if true.
  *
  * Reference:
  * http://stackoverflow.com/questions/4770985/something-like-startswithstr-a-str-b-in-c
  */
int startsWith(const char *pre, const char *str);

/**
  * Reference:
  * http://stackoverflow.com/questions/744766/how-to-compare-ends-of-strings-in-c
  */
int endsWithPac(const char *str);

/**
  * Function for automatically filling all the appropriate place with a simple character
  */
void auto_fill_pellet(const int current_x, const int current_y, const char direction);

/**
  * Function for searching for the first orcurence of pacman spawn point in the map. if there is no spawn point the function will return an errorl
  */
void search_pacman_spawn_point(int *pacman_spawn_point_x, int *pacman_spawn_point_y);

/**
  * Function to validate if author name is entered correctly
  */
int isValidAuthor(char * author_arg);

/**
  * Function to determined the input cell is valid to fill with small pellet or not
  */
int isValidFillCell(const int current_x, const int current_y);

/**
  * Replace all the character that mark the possilble small pellet place with the actual small pellet character
  */
void  replace_marked_point();

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


int main(int argc, char *argv[]) {
    int input;

    /* get terminal size */
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    /* default values for author, title, file name, height and width */
    author = malloc(sizeof(char) * (strlen(DEFAULT_AUTHOR) + 1));
    strcpy(author, DEFAULT_AUTHOR);

    title = malloc(sizeof(char) * (strlen(DEAFULT_TITTLE) + 1));
    strcpy(title, DEAFULT_TITTLE);

    file_name = malloc(sizeof(char) * (strlen(DEAFULT_FILE_NAME) + 1));
    strcpy(file_name, DEAFULT_FILE_NAME);

    /* set default height and width */
    height = DEFAULT_HEIGHT;
    width = DEFAULT_WIDTH;

    /* determine the current working directory and the levels folder for save map */
    if (startsWith("./src/", argv[0])) {
        directory = DIRECTORY_SH;
    } else {
        directory = DIRECTORY_SRC;
    }
    
    /* initialise ncurses screen */
    initscr();

    /* method are given from http://en.chys.info/2009/09/esdelay-ncurses/ */
    if (getenv ("ESCDELAY") == NULL)
        ESCDELAY = 25;

    /* enable the use of function keys, allow navigating the cursor using arrow keys */
    keypad(stdscr, TRUE);

    /* disable echo when getch */
    noecho();
 
    /* take input chars, does not wait until new line or carriage return */
    cbreak();
    
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

    /* if there is an argument for the program */
    if (argc == 2) {
    	/* get the path to the file */
        char path[strlen(directory) + strlen(argv[1]) + 1];
        strcpy(path, directory);
        strcat(path, argv[1]);

        /* try to get open the file */
        FILE *f = fopen(path, "r");

        /* if there is a file with the path */
        if (f != NULL){
        	/* red the file */
        	fclose(f);
            read_file(argv[1]);
            display_map(map);
        /* if the is no file with the path */
        } else {
        	/* create a new map with default height and width and assign the file name with the argument */
            map = create_map(height, width);

            free(file_name);
            file_name = malloc(sizeof(char) * (strlen(argv[1]) + 1));
            strcpy(file_name, argv[1]);
        }
    /* if there is no argunment provided */
    } else {        
        map = create_map(height, width);
    }
    
    /* Run until program is ended */
    while(!end_program) {
    	/* refresh the terinal size */
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        /* clear the screen if message error count is 0, to clear the error message */
        if (error_msg_count <= 0) {            
            clear();
        }

        /* display current map from the memory */
        display_map(map);

        /* get input command */
        input = getch();

        /* check for ":" to enter command mode */
        if (input == ':') {
        	/* if message error count is not 0 then decrase it */
            if (error_msg_count > 0) {
                error_msg_count--;
            } else {            
                move(w.ws_row - 2, 0);
                clrtobot();
            }

            command_mode();
        } else if (error_msg_count > 0) {
            error_msg_count--;
        }

        edit_mode(input);
    }

    /* close the terminal */
    endwin();

    /* free the memory */
    free(map);
    free(author);
    free(title);
    free(file_name);

    /* exit the program */
    exit(0);
}

void command_mode() {
	/* character array to store the command and its arguments */
    char args[1000];
    char command[1000];

    /* number of characters in the command string */
    int count = 0;

    /* pressed key on the keyboard */
    int input;

    /* display the input characters on the screen */
    echo();

    /* print the ":" character at the bottom of screen indicate the command mode is activated */
    mvprintw(w.ws_row - 1, 0, ":");

    /* get the pressed key from keyboard */
    input = getch();

    /* if pressed key is not "Enter" or "Escape" */
    while (input != 27 && input != 10) {

    	/* move the cursor right 1 character */
        move(w.ws_row - 1, count + 1);

        /* if "Backspace" is pressed */
        if (input == KEY_BACKSPACE  && count != 0) {
        	/* move cursor left 1 character and clear everything on the right */
            move(w.ws_row - 1, count);
            clrtoeol();
            count--;

            /* wait for another input */
            input = getch();

            /* run another iteration of the loop */
            continue;
        }
        
        /* if key other than "Left", "Right", "Up", "Down" and "Backspace" is pressed */
        if (input != KEY_UP && input != KEY_DOWN && input != KEY_LEFT && input != KEY_RIGHT && input != KEY_BACKSPACE) {

        	/* move the cursor to the right 1 character */
            move(w.ws_row - 1, count + 2);

            /* save the input character to the memory */
            command[count] = input;

            /* increase the command lengthl */
            count++;
        }

        /* wait for another input */
        input = getch();       
    }
    
    /* if "Escape" is pressed */
    if (input == 27) {
    	/* clear evething on the command input now */
        move(w.ws_row - 1, 0);
        clrtoeol();
    }

    /* clear the command string */
    command[count] = '\0';

    /* if "Enter" is pressed */
    if (input == 10) {
    	/* clear all character in the command input */
        clrtoeol();
        switch (strlen(command)) {
            /* check for user command */
            case 1:
            	/* quit command */
                if(strcmp(command, "q") == 0) {
                    end_program = 1;
                    return;
                /* save current map with current file name */
                } else if (strcmp(command,"w") == 0) {
                    write_file(file_name);
                } 
                break;
            case 2:
            	/* save current map with current file name and quit */
                if (strcmp(command,"wq") == 0) {
                    write_file(file_name);
                    end_program = 1;
                }
                break;
            default:
            	/* save with custom name and quit */
                if (startsWith("wq ", command) != 0) {
                	/* copy the character in the command array to args and convert to string */
                    memcpy(args, &command[3], strlen(command));
                    args[strlen(command) - 3] = '\0';

                    /* write file with args as file name */
                    write_file(args);
                    end_program = 1;
                /* save with custom name */
                } else if(startsWith("w ", command) != 0) {
                	/* copy the character in the command array to args and convert to string */
                    memcpy(args, &command[2], strlen(command));
                    args[strlen(command) - 2] = '\0';

                    /* write file with args as file name */
                    write_file(args);
                /* read the file */
                } else if(startsWith("r ", command) != 0) {
                	/* copy the character in the command array to args and convert to string */
                    memcpy(args, &command[2], strlen(command));
                    args[strlen(command) - 2] = '\0';

                    /* read file with args as file name */
                    read_file(args);
                    display_map(map);
                /* create new map */
                } else if(startsWith("n ", command) != 0) {
                	/* copy the character in the command array to args and convert to string */
                    memcpy(args, &command[2], strlen(command));
                    args[strlen(command) - 2] = '\0';

                    /* create new map with args */
                    new_map(args);
                    display_map(map);
                /* edit author */
                } else if(startsWith("a ", command) != 0) {
                	char * temp = malloc(sizeof(char) * (strlen(command) - 1));
                	memcpy(temp, &command[2], strlen(command));
                	temp[strlen(command) - 2] = '\0';

                	if (isValidAuthor(temp)) {

                		/* free the current author */
                    	free(author);
                    	author = malloc(sizeof(char) * (strlen(temp) + 1));

                    	/* copy the character in the command array to args and convert to string */
                    	strcpy(author, temp);
                	}
                /* edit tittle */
                } else if (startsWith("t ", command) != 0) {
                	/* free the current author */
                    free(title);
                    title = malloc(sizeof(char) * (strlen(command) - 1));

                    /* copy the character in the command array to args and convert to string */
                    memcpy(title, &command[2], strlen(command));

                    /* set new tittle */
                    title[strlen(command) - 2] = '\0';
                /* auto fill available space with small pellet */
                } else if (strcmp(command,"auto") == 0) {
                	/* initialize pacman spawn point */
                	int pacman_spawn_point_x = -1;
					int pacman_spawn_point_y = -1;

					/* search for pacman spawn point */
                	search_pacman_spawn_point(&pacman_spawn_point_x, &pacman_spawn_point_y);

                	/* fill space starting at the pacman spawn point */
                	auto_fill_pellet(pacman_spawn_point_x, pacman_spawn_point_y, ' ');

                	 replace_marked_point();
                }
        }
    }

    /* disable display input characters on the screen */
    noecho();
}

void display_map(char* map) {
    int count = 0;

    /* calculate the offset to display the map on screen */
    x_offset = (((w.ws_row - 6) / 2) - (height / 2)) > 0 ? ((w.ws_row -6) / 2) - (height / 2): 0;
    y_offset = ((w.ws_col / 2) - (width / 2)) > 0 ? (w.ws_col / 2) - (width / 2): 0;

    /* set normal color */
    attrset(COLOR_PAIR(1));

    /* display map general information */
    move(0, 0);
    clrtoeol();
    attrset(COLOR_PAIR(9));
    printw("Map file name: ");
    attrset(COLOR_PAIR(8));
    printw(file_name);
    attrset(COLOR_PAIR(1));

    move(1, 0);
    clrtoeol();
    attrset(COLOR_PAIR(9));
    printw("Author:        ");
    attrset(COLOR_PAIR(8));
    printw(author);
    attrset(COLOR_PAIR(1));

    move(2, 0);
    clrtoeol();
    attrset(COLOR_PAIR(9));
    printw("Map title:     ");
    attrset(COLOR_PAIR(8));
    printw(title);
    attrset(COLOR_PAIR(1));

    /* draw 2 horrizontal line ont the screen */
    attrset(COLOR_PAIR(9));
    for (int i = 0; i < w.ws_col; i++) {
    	mvprintw(3, i, "-");
    	mvprintw(w.ws_row - 3, i, "-");
    }
    attrset(COLOR_PAIR(1));

    /* move */
    move(4 + x_offset, 0 + y_offset);

    attrset(COLOR_PAIR(3));

    /* display character base on character stored in memory */
    for (int i = 0; i < height * width; i++) {

        if (i != 0 && i % width == 0) {
            move(4 + x_offset + (i / width), y_offset);
        }

        switch (map[i]) {
            case 'q':
            case 'Q':
                addch(ACS_ULCORNER);
                break;
            case 'w':
            case 'x':
                addch(ACS_HLINE);
                break;
            case 'e':
            case 'E':
                addch(ACS_URCORNER);
                break;
            case 'a':
            case 'd':
                addch(ACS_VLINE);
                break;
            case 's':
                attrset(COLOR_PAIR(4));
                addch(ACS_BULLET);
                attrset(COLOR_PAIR(3));
                break;
            case 'S':
                attrset(COLOR_PAIR(6));
                addch(ACS_DEGREE);
                attrset(COLOR_PAIR(3));
                break;
            case 'z':
            case 'Z':
                addch(ACS_LLCORNER);
                break;        
            case 'c':
            case 'C':
                addch(ACS_LRCORNER);
                break;        
            case 'W':
                addch(ACS_TTEE);
                break;
            case 'X':
                addch(ACS_BTEE);
                break;
            case 'A':
                addch(ACS_LTEE);
                break;
            case 'D':
                addch(ACS_RTEE);
                break;
            case 'g':
            case 'G':                
                attrset(COLOR_PAIR(4 + count));
                addch(ACS_PI);
                attrset(COLOR_PAIR(3));

                if (count <= 3) {
                    count++;
                } else {
                    count = 0;
                }

                break;
            case 'p':
            case 'P':
                attrset(COLOR_PAIR(4));
                addch(ACS_STERLING);
                attrset(COLOR_PAIR(3));
                break;
            case 'f':
            case 'F':
                attrset(COLOR_PAIR(5));
                addch(ACS_DIAMOND);
                attrset(COLOR_PAIR(3));
                break;
            default:
                addch(' ');
                break;
        }
    }

    attrset(COLOR_PAIR(1));

    move(x + 4 + x_offset, y + y_offset);
}

void edit_mode(int input) {

	/* check for pressed key, if it is "Left", "Right", "Up" and "Down" then move the cursor, if other then stored it on memory */
    switch(input) {
        case KEY_UP:
            if(x > 0)
                x--;
            break;
        case KEY_DOWN:
            if(x < height - 1)
                x++;
            break;
        case KEY_LEFT:
            if(y > 0)
                y--;
            break;
        case KEY_RIGHT:
            if(y < width - 1)
                y++;
            break;
        case 'q':
        case 'Q':
        case 'w':
        case 'x':
        case 'e':
        case 'E':
        case 'a':
        case 'd':
        case 'W':
        case 'X':
        case 'A':
        case 'D':
        case 's':
        case 'S':
        case 'z':
        case 'Z':
        case 'c':
        case 'C':
        case 'g':
        case 'G':
        case 'p':
        case 'P':
        case 'f':
        case 'F':
        case ' ':
            map[x * width + y] = input;
            break;
        default:
            break;
    }

    move(x + 4 + x_offset, y + y_offset);
}

void new_map(char* args) {

	/* temp size for the map */
    int temp_height;
    int temp_width;

    int count = 0;
    int error = 0;

    char *token = NULL;
    char *temp_file_name = NULL;
    char *arguments[] = {"file name", "map height", "map width"};

    /* split the input args by token */
    token = strtok(args, " ");
    while (token != NULL) {
    	/* first argument */
        if (count == 0) {
        	/* check if contains other than numbers, letters and punctuation */
            reti = regcomp(&regex,"^[[:alnum:][:punct:]]", 0);
            reti = regexec(&regex, token, 0, NULL, 0);

            if (reti == REG_NOMATCH) {
                attrset(COLOR_PAIR(2));
                mvprintw(w.ws_row - 2, 0, "Please specify the file name correctly");
                error = 1;
                attrset(COLOR_PAIR(1));
                break;    
            }

            regfree(&regex);
            temp_file_name = token;
        /* second argument */
        } else if (count == 1) {

        	/* check if it is a number or not */
            reti = regcomp(&regex,"^[[:digit:]]", 0);
            reti = regexec(&regex, token, 0, NULL, 0);    
            if (reti == REG_NOMATCH) {
                attrset(COLOR_PAIR(2));
                mvprintw(w.ws_row - 2, 0, "Please specify the number of rows correctly");
                error = 1;
                attrset(COLOR_PAIR(1));
                break;
            }

            regfree(&regex);
            sscanf(token, "%d", &temp_height);

            /* check if it is in acceptable range */
            if (temp_height > w.ws_row - 8 || temp_height < 1) {
                attrset(COLOR_PAIR(2));
                mvprintw(w.ws_row - 2, 0, "Please enter a valid height (1 to %d)", w.ws_row - 8);
                error = 1;
                attrset(COLOR_PAIR(1));
                break;            
            }
        /* third argument */
        } else if (count == 2) {

        	/* check if it is a number or not */
            reti = regcomp(&regex,"^[[:digit:]]", 0);
            reti = regexec(&regex, token, 0, NULL, 0);
                
            if (reti == REG_NOMATCH) {
                attrset(COLOR_PAIR(2));
                mvprintw(w.ws_row - 2, 0, "Please specify the number of column correctly");
                error = 1;
                attrset(COLOR_PAIR(1));
                break;
            }

            regfree(&regex);
            sscanf(token, "%d", &temp_width);

            /* check if it is in acceptable range */
            if (temp_width > w.ws_col - 1 || temp_width < 1) {
                attrset(COLOR_PAIR(2));
                mvprintw(w.ws_row - 2, 0, "Please enter a valid width (1 to %d)", w.ws_col - 1);
                error = 1;
                attrset(COLOR_PAIR(1));
                break;            
            }
        } else {
            break; 
        }

        token = strtok(NULL, " ");
        count++;
    }
    
    /* if there is not enough argument provided */
    if (count < 2 && error != 1) {
    	attrset(COLOR_PAIR(2));
        mvprintw(w.ws_row - 2, 0, "Not enough arguments, need: file name, map height and map width");

        mvprintw(w.ws_row - 1, 0, "Found: %s", arguments[0]);

        for (int i = 1; i < count; i++) {
        	printw(", %s", arguments[i]);
        }

        error = 1;
        attrset(COLOR_PAIR(1));
    }

    if (error == 0) {
    	/* update file name */
    	free(file_name);
        if (endsWithPac(temp_file_name)) {
            file_name = malloc(sizeof(char) * (strlen(temp_file_name) + 1));
            strcpy(file_name, temp_file_name);
        } else {
            file_name = malloc(sizeof(char) * (strlen(temp_file_name) + 5));
            strcpy(file_name, temp_file_name);
            strcat(file_name, ".pac");
        }

        /* create map with height and width */
       	map = create_map(temp_height, temp_width);
       	clear();
	   	move(x + 4 + x_offset, y + y_offset);
    } else {
        error_msg_count = 1;
    }
}

void write_file(char* file) {
	/* check if contains other than numbers, letters and punctuation */
    reti = regcomp(&regex,"^[[:alnum:][:punct:]]", 0);
    reti = regexec(&regex, file, 0, NULL, 0);

    if (reti == REG_NOMATCH) {
        attrset(COLOR_PAIR(2));
        mvprintw(w.ws_row - 2, 0, "Please specify the file name to be save or use 'w' to save the file with default name");
        attrset(COLOR_PAIR(1));
        error_msg_count = 1;
    } else {
    	int length = strlen(file);
    	char * temp = malloc(sizeof(char) * (length + 1));
    	strcpy(temp, file);

    	/* update file name */
    	free(file_name);
        if (endsWithPac(temp)) {
            file_name = malloc(sizeof(char) * (length + 1));
            strcpy(file_name, temp);
        } else {
            file_name = malloc(sizeof(char) * (length + 5));
            strcpy(file_name, temp);
            strcat(file_name, ".pac");
        }

        free(temp);

        /* contruct file path */
        char path[strlen(directory) + strlen(file_name) + 1];
        strcpy(path, directory);
        strcat(path, file_name);

        int i;

        /* open and write file */
        FILE *f = fopen(path, "w");

       	fprintf(f, "%s\n", author);
       	fprintf(f, "%s\n%i\n%i\n", title, height, width);

       	for (i = 0; i < height * width; i++) {
           	if (i % width == 0 && i != 0)
               	fprintf(f,"\n");
           	fprintf(f,"%c", map[i]);
       	}

       	fprintf(f, "\n");

       	fclose(f);
        
        error_msg_count = 0;
    }
    regfree(&regex);
}

void read_file(char * file) {
	/* check if contains other than numbers, letters and punctuation */
    reti = regcomp(&regex,"^[[:alnum:][:punct:]]", 0);
    reti = regexec(&regex, file, 0, NULL, 0);
    if (reti == REG_NOMATCH) {
        attrset(COLOR_PAIR(2));
        mvprintw(w.ws_row - 2, 0, "Please specify the file name to be open");
        attrset(COLOR_PAIR(1));
        error_msg_count = 1;
    } else  {
        int temp_height;
        int temp_width;

        size_t len;
        char *temp = NULL;
        char *temp_author = NULL;
        char *temp_title = NULL;

        /* update file name*/
        free(file_name);
        file_name = malloc(sizeof(char) * (strlen(file) + 1));
        strcpy(file_name, file);

        /* contruct file path */
        char path[strlen(directory) + strlen(file) + 1];
        strcpy(path, directory);
        strcat(path, file);

        /* open the file and read from it */
        FILE *f = fopen(path, "r");

        if (f == NULL) {
            attrset(COLOR_PAIR(2));
            mvprintw(w.ws_row - 2, 0,"No such file found");
            attrset(COLOR_PAIR(1));
            error_msg_count = 1;
           return;
        }
                
        getline(&temp_author, &len, f);
        getline(&temp_title, &len, f);

        /* update author */
        free(author);
        author = malloc(sizeof(char) * (strlen(temp_author)));
        memcpy(author, &temp_author[0], strlen(temp_author) - 1);
        author[strlen(temp_author) - 1] = '\0';

        /* update tittle */
        free(title);
        title = malloc(sizeof(char) * (strlen(temp_title)));
        memcpy(title, &temp_title[0], strlen(temp_title) - 1);
        title[strlen(temp_title) - 1] = '\0';

        getline(&temp, &len, f);
        sscanf(temp, "%d", &temp_height);

        getline(&temp, &len, f);
        sscanf(temp, "%d", &temp_width);

        map = create_map(temp_height, temp_width);

        for (int row = 0; row < temp_height; row++) {
            getline(&temp, &len, f);

            for (int col = 0; col < temp_width; col++) {
                map[row * width + col] = temp[col];
            }
        }

        /* free memory and clode the file */
        free(temp);
        free(temp_author);
        free(temp_title);

        fclose(f);
        clear();
    }
}

char *create_map(int new_height, int new_width) {
    height = new_height;
    width = new_width;

    /* calculate the offset to display the map on screen */
    x_offset = (((w.ws_row - 6) / 2) - (height / 2)) > 0 ? ((w.ws_row -6) / 2) - (height / 2): 0;
    y_offset = ((w.ws_col / 2) - (width / 2)) > 0 ? (w.ws_col / 2) - (width / 2): 0;

    x = 0;
    y = 0;

    /* free the current map */
    if (map) {
        free(map);
    }
    map = malloc(sizeof(char) * (height * width));

    /* file all current character with space */
    for (int i = 0; i < (height * width); i++) {
        map[i] = ' ';
    }

    /* draw vertical border */
    for (int row = 1; row < height - 1; row++) {
        map[row * width] = 'a';
        map[row * width + width - 1] = 'd';
    }

    /* draw horrizontal border */
    for (int col = 1; col < width - 1; col++) {
        map[col] = 'w';
        map[width * (height - 1) + col] = 'x';
    }

    /* draw corner border */
    map[0] = 'q';
    map[width - 1] = 'e';
    map[(height - 1) * width] = 'z';
    map[height * width - 1] = 'c';

    return map;
}

int startsWith(const char *pre, const char *str) {
  size_t lenpre = strlen(pre),
  lenstr = strlen(str);
  return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

int endsWithPac(const char *str) {
  if (!str)
    return 0;
  
  size_t lenstr = strlen(str);
  size_t lensuffix = strlen(".pac");

  if (lensuffix >  lenstr)
    return 0;

  return strncmp(str + lenstr - lensuffix, ".pac", lensuffix) == 0;
}

void auto_fill_pellet(const int current_x, const int current_y, const char direction) {

	switch (map[(current_x) * width + current_y]) {
		case ' ':
			map[(current_x) * width + current_y] = 'o';
			break;
		case 's':
		case 'S':
		case 'f':
		case 'F':
		case 'g':
		case 'G':
		case 'p':
		case 'P':
			break;
		default:
			return;
	}

	/* recursively call itself in other cell after fill it with 's' if it contains ' ' */

	if (isValidFillCell(current_x + 1, current_y) && direction != 'u') {
//		map[(current_x + 1) * width + current_y] = 's';
		auto_fill_pellet(current_x + 1, current_y, 'd');
	}
	if (isValidFillCell(current_x, current_y + 1) && direction != 'l') {
//		map[current_x * width + current_y + 1] = 's';
		auto_fill_pellet(current_x, current_y + 1, 'r');
	}
	if (isValidFillCell(current_x - 1, current_y) && direction != 'd') {
//		map[(current_x - 1) * width + current_y] = 's';
		auto_fill_pellet(current_x - 1, current_y, 'u');
	}
	if (isValidFillCell(current_x, current_y - 1) && direction != 'r') {
//		map[current_x * width + current_y - 1] = 's';
		auto_fill_pellet(current_x, current_y - 1, 'l');
	}
}

void  replace_marked_point() {
	for (int i = 0; i < height * width; i++)
	{
		if (map[i] == 'o') {
			map[i] = 's';
		}
	}
}

int isValidFillCell(const int current_x, const int current_y) {
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

void search_pacman_spawn_point(int *pacman_spawn_point_x, int *pacman_spawn_point_y) {
	/* search for pacman spawn point */
	for (int i = 0; i < (height * width); i++) {
		if (map[i] == 'p' || map[i] == 'P') {
			*pacman_spawn_point_x = i / width;
			*pacman_spawn_point_y = i % width;
			break;
		}
	}

	/* display error message if no spawn point found */
	if (*pacman_spawn_point_x == -1 || *pacman_spawn_point_y == -1) {
		attrset(COLOR_PAIR(2));
        mvprintw(w.ws_row - 2, 0, "No pacman spawn point found.\nPlease specify at least 1 spawn point for pacman.");
        attrset(COLOR_PAIR(1));
        error_msg_count = 1;
	}
}

int isValidAuthor(char * author_arg) {
	char *temp = malloc(sizeof(char)*strlen(author_arg));
	strcpy(temp,author_arg);
	char *token = NULL;

	token = strtok(temp, ",");
	while (token != NULL) {
		reti = regcomp(&regex,"^[[:alpha:][:space:]]*[[:space:]]<[[:graph:]]*@[[:punct:][:alpha:]]*>", 0);
        reti = regexec(&regex, token, 0, NULL, 0);

        if (reti == REG_NOMATCH) {
            attrset(COLOR_PAIR(2));
            mvprintw(w.ws_row - 2, 0, "Invalid author");
            error_msg_count = 1;
            attrset(COLOR_PAIR(1));
            return 0;
        }

		token = strtok(NULL, ",");
	}
	return 1;
}
