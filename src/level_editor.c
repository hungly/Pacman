#include "level_editor.h"
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
#define DEFAULT_TITTLE          "Unknown"

/** Define the defalut file name */
#define DEFAULT_FILE_NAME       "level.pac"

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

int editor(int argc, char *argv[]) {
    int input;

    /* get terminal size */
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    /* default values for author, title, file name, height and width */
    author = malloc(sizeof(char) * (strlen(DEFAULT_AUTHOR) + 1));
    memcpy(author, &DEFAULT_AUTHOR[0], strlen(DEFAULT_AUTHOR));
    author[strlen(DEFAULT_AUTHOR)] = '\0';

    title = malloc(sizeof(char) * (strlen(DEFAULT_TITTLE) + 1));
    memcpy(title, &DEFAULT_TITTLE[0], strlen(DEFAULT_TITTLE));
    title[strlen(DEFAULT_TITTLE)] = '\0';

    file_name = malloc(sizeof(char) * (strlen(DEFAULT_FILE_NAME) + 1));
    memcpy(file_name, &DEFAULT_FILE_NAME[0], strlen(DEFAULT_FILE_NAME));
    file_name[strlen(DEFAULT_FILE_NAME)] = '\0';

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
          if(map){
            free(map);
          }
        	/* create a new map with default height and width and assign the file name with the argument */
          map = create_map(height, width);

          free(file_name);
          file_name = malloc(sizeof(char) * (strlen(argv[1]) + 1));
          memcpy(file_name, &argv[1][0], strlen(argv[1]));
          file_name[strlen(argv[1])] = '\0';
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
                      memcpy(author, &temp[0], strlen(temp));
                      author[strlen(temp)] = '\0';
                	}
                /* edit tittle */
                } else if (startsWith("t ", command) != 0) {
                	/* free the current author */
                    free(title);
                    title = malloc(sizeof(char) * (strlen(command) - 1));

                    /* copy the character in the command array to args and convert to string */
                    memcpy(title, &command[2], strlen(command));
                    title[strlen(command) - 2] = '\0';
                /* auto fill available space with small pellet */
                } else if (strcmp(command, "autoremove") == 0) {
                	replace_marked_point('s', ' ');
                } else if (strcmp(command, "auto") == 0) {
                	/* initialize pacman spawn point */
                	int pacman_spawn_point_x = -1;
					int pacman_spawn_point_y = -1;

					/* search for pacman spawn point */
                	search_pacman_spawn_point(&pacman_spawn_point_x, &pacman_spawn_point_y);

                	/* fill space starting at the pacman spawn point */
                	auto_fill_pellet(pacman_spawn_point_x, pacman_spawn_point_y, ' ');

                	replace_marked_point('o', 's');
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
            file_name[strlen(temp_file_name)] = '\0';
        } else {
            file_name = malloc(sizeof(char) * (strlen(temp_file_name) + 5));
            strcpy(file_name, temp_file_name);
            strcat(file_name, ".pac");
            file_name[strlen(temp_file_name) + 4] = '\0';
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
    	int length = strlen(file) + 1;
    	char * temp = malloc(sizeof(char) * (length));
    	strcpy(temp, file);
      	temp[length] = '\0';

    	/* update file name */
    	free(file_name);
        if (endsWithPac(temp)) {
            file_name = malloc(sizeof(char) * (length));
            strcpy(file_name, temp);
            file_name[length] = '\0';
        } else {
            file_name = malloc(sizeof(char) * (length + 4));
            strcpy(file_name, temp);
            strcat(file_name, ".pac");
            file_name[length + 4] = '\0';
        }

        free(temp);

        /* contruct file path */
        char path[strlen(directory) + strlen(file_name)];
        strcpy(path, directory);
        strcat(path, file_name);

        int i;

        /* open and write file */
        FILE *f = fopen(path, "w+");

       	fprintf(f, "%s\n", author);
       	fprintf(f, "%s\n%i\n%i\n", title, height, width);

       	for (i = 0; i < height * width; i++) {
           	if (i % width == 0 && i != 0)
               	fprintf(f, "\n");
           	fprintf(f, "%c", map[i]);
       	}

       	fprintf(f, "\n\n");

       	fclose(f);
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
        file_name[strlen(file)] = '\0';

        /* construct file path */
        char path[strlen(directory) + strlen(file)];
        strcpy(path, directory);
        strcat(path, file);

        /* open the file and read from it */
        FILE *f = fopen(path, "r");

        if (f == NULL) {
            attrset(COLOR_PAIR(2));
            mvprintw(w.ws_row - 2, 0, "No such file found");
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

        /* free memory and close the file */
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
		auto_fill_pellet(current_x + 1, current_y, 'd');
	}
	if (isValidFillCell(current_x, current_y + 1) && direction != 'l') {
		auto_fill_pellet(current_x, current_y + 1, 'r');
	}
	if (isValidFillCell(current_x - 1, current_y) && direction != 'd') {
		auto_fill_pellet(current_x - 1, current_y, 'u');
	}
	if (isValidFillCell(current_x, current_y - 1) && direction != 'r') {
		auto_fill_pellet(current_x, current_y - 1, 'l');
	}
}

void  replace_marked_point(const char original, const char replacement) {
	for (int i = 0; i < height * width; i++)
	{
		if (map[i] == original) {
			map[i] = replacement;
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
            mvprintw(w.ws_row - 2, 0, "Invalid author(s). Please enter each author with 'name <email>'");
            error_msg_count = 1;
            attrset(COLOR_PAIR(1));
            return 0;
        }

		token = strtok(NULL, ",");
	}
	return 1;
}
