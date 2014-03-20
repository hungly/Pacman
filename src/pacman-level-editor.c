#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <regex.h>

#define DEFAULT_HEIGHT          10
#define DEFAULT_WIDTH           30
#define DIRECTORY               "levels/"
#define DEFAULT_AUTHOR          "Anonymous"
#define DEAFULT_TITTLE          "Unknown"
#define DEAFULT_FILE_NAME       "level.pac"

#define COLOR_BACKGROUND        0
#define COLOR_ERROR_BACKGROUND  1
#define COLOR_NORMAL            2
#define COLOR_WALL              3
#define COLOR_PACMAN            4
#define COLOR_FRUIT             5
#define COLOR_GHOST_1           6
#define COLOR_GHOST_2           7
#define COLOR_GHOST_3           8
#define COLOR_GHOST_4           9

/**
  * VI liked command mode for map pacman editor.
  * This function will check if the input command is either q, w, n, r or wq and performed the following actions:
  * q: quit the program and exit to the terminal.
  * w: save the current map to the levels directory, this command accepts a parameter as the file name, if there is no file name, the map will be saved using the default name "level.pac".
  * wq: save and then quit the program, this command perform the same actions as the "w" command and then the "q" command.
  * n: create a new map, this command take three parameters, the first parameter is the new map name, the second and third parameters specify the height and width of the new map respectively.
  * r: load a map from the levels directory into the map editor, this command accept one parameter specify the file name of the map.
  */
void command_mode();

/** Display a map using an array
  * @param char* map is the pointer to the array of character
  */
void display_map(char* map);

/** Handling changes in editing map mode 
  * @param int input the is key code of the input in the ascii code
  */
void edit_mode(int input);

/** Handling new map creation using :n in command mode
  * @param char* args is pointer to the array of character, which includes new file name, height, and width
  */
void new(char* args);

/** Writing map information to file 
  * @param char* file is pointer to the array of character, which indicates the file name to write
  */
void write_file(char* file);

/** Reading map information from file 
  * @param char* file is pointer to the array of character, which indicates the file name to read
  */
void read_file(char* file);

/** Storing the initial map 
  * @param height is the height of map
  * @param width is the width of map
  * Return pointer to an array of char which represent the map 
  */
char *create_map(int height, int width);

/** 
  * Display current loaded map from an array in the heap memory.
  *
  * @param char * map the pointer to the array of character using to represent the current loaded map.
  */
void display_map(char * map);

/**
  * Handling input that change the current edited map.
  *
  * @param int input the ASCII code of the input command.
  */
void edit_mode(int input);

/**
  * Handling new map creation using with n command in command mode and when the program is loaded.
  *
  * @param char * args the pointer to the array of characters which includes all three parameters which are new file name, height, and width.
  */
void new(char * args);

/**
  * Writing the current map and related information into a file.

  * @param char * file the pointer to the array of characters which indicates the file name to write into the disk.
  */
void write_file(char*  file);

/**
  * Reading the current map and related information into a file.
  *
  * @param char* file the pointer to the array of characters which indicates the file name to read from the disk.
  */
void read_file(char * file);

/**
  * Create a new map with desire height and width.
  *
  * @param height is the height of map.
  * @param width is the width of map.
  *
  * Return pointer to an array of characters which represent the map .
  */
char *create_map(int height, int width);

/**
  * Function for checking the beginning of the string
  *
  * @param const char *pre is the pointer to an array of char which we want to check whether the string begin with that or not.
  * @param const char *str is the pointer to an array of char which is the string we want to check.
  *
  * Return 0 if false and other numbers if true.
  *
  * Reference:
  * http://stackoverflow.com/questions/4770985/something-like-startswithstr-a-str-b-in-c
  */
int startsWith(const char *pre, const char *str);

char *new_map;
char *map;

char *author;
char *title;
char *file_name;

int end_program = 0;
int height;
int width;
int x = 0;
int y = 0;
int error_msg_count = 1;

/** regex is used to to check for input pattern */
regex_t regex;
int reti;

/** regex is used to to check for input pattern */
regex_t regex;
int reti;


int main(int argc, char* argv[])
{
	int input;

	/** default values for author, title, file name, height and width*/
	author = malloc(sizeof(char) * (strlen(DEFAULT_AUTHOR) + 1));
 	memcpy(author, &DEFAULT_AUTHOR[0], strlen(DEFAULT_AUTHOR));
  	author[strlen(DEFAULT_AUTHOR)] = '\0';

  	title = malloc(sizeof(char) * (strlen(DEAFULT_TITTLE) + 1));
  	memcpy(title, &DEAFULT_TITTLE[0], strlen(DEAFULT_TITTLE));
  	title[strlen(DEAFULT_TITTLE)] = '\0';

  	file_name = malloc(sizeof(char) * (strlen(DEAFULT_FILE_NAME) + 1));
  	memcpy(file_name, &DEAFULT_FILE_NAME[0], strlen(DEAFULT_FILE_NAME));
  	file_name[strlen(DEAFULT_FILE_NAME)] = '\0';

	height = DEFAULT_HEIGHT;
	width = DEFAULT_WIDTH;

  	
	/** initialise ncurses screen */
	initscr();

	if (getenv ("ESCDELAY") == NULL)
 		ESCDELAY = 25;
	/** enable the use of function keys, allow navigating the cursor using arrow keys */
	keypad(stdscr, TRUE);

	/** disable echo when getch */
	noecho(); 
	/** take input chars, does not wait until new line or carriage return */
	cbreak();

	
    if(has_colors())
    {
        start_color();
        init_pair(1, COLOR_NORMAL,  COLOR_BACKGROUND);
        init_pair(2, COLOR_NORMAL,  COLOR_ERROR_BACKGROUND);
        init_pair(3, COLOR_WALL,    COLOR_BACKGROUND);
        init_pair(4, COLOR_PACMAN,  COLOR_BACKGROUND);
        init_pair(5, COLOR_FRUIT,   COLOR_BACKGROUND);
        init_pair(6, COLOR_GHOST_1, COLOR_BACKGROUND);
        init_pair(7, COLOR_GHOST_2, COLOR_BACKGROUND);
        init_pair(8, COLOR_GHOST_3, COLOR_BACKGROUND);
        init_pair(9, COLOR_GHOST_4, COLOR_BACKGROUND);
    }

   	if (argc == 2)
   	{
   		char path[strlen(DIRECTORY) + strlen(argv[1]) + 1];
   		strcpy(path,DIRECTORY);
   		strcat(path,argv[1]);
   		FILE *f = fopen(path,"r");
   		if (f != NULL){
	   		read_file(argv[1]);
   			display_map(map);
   		} else {
   			map = create_map(height, width);
   		}

   	} 
   	else 
   	{
		map = create_map(height, width);
	}
	
	
	/** run until program is ended */
	while(!end_program)
	{
        if (error_msg_count <= 0)
        {            
            move(height + 5, 0);
            clrtoeol();
        }

  		/** display current map from the memory */
		display_map(map);

    	/** get input command */
		input = getch();

    /** check for ":" to enter command mode */
		if (input == ':')
		{
            if (error_msg_count > 0)
            {
                error_msg_count--;
            }
			command_mode();
		}
		else
		{
            if (error_msg_count > 0)
            {
                error_msg_count--;
            }
			edit_mode(input);
		}
	}
	endwin();

  free(map);
  free(author);
  free(title);
  free(file_name);

  exit(0);
}

void command_mode()
{
	char args[1000];

	int count = 0;
	int input;

	char command[1000];

    /** display the input characters on the screen */
	echo();

	mvprintw(height + 1, 0, ":");

	input = getch();
	while (input != 27 && input != 10)
	{
        move(height + 1, count + 1);

		if (input == KEY_BACKSPACE  && count != 0)
		{
			mvprintw(height + 1, count, "     ");
            move(height + 1, count);
            count--;
            input = getch();
            continue;
		}
		
        if (input != KEY_UP && input != KEY_DOWN && input != KEY_LEFT && input != KEY_RIGHT && input != KEY_BACKSPACE)
        {
            move(height + 1, count + 2);

            command[count] = input;

            count++;
        }
				
        input = getch();       
	}
	
	if (input == 27){
		move(height+1,0);
		clrtoeol();
	}
	command[count] = '\0';

    if (input == 10)
    {
        clrtoeol();
        switch (strlen(command)){
        	/** check for user input in command mode */
            case 1:
                if(strcmp(command, "q") == 0)
                {
                    end_program = 1;
                    return;
                } else if (strcmp(command,"w") == 0) {
                    write_file(file_name);
                }
                break;
            case 2:
                if (strcmp(command,"wq") == 0)
                {
                    write_file("level.pac");
                    end_program = 1;
                }
                break;
            default:
                if (startsWith("wq ", command) != 0)
                {
                    memcpy(args, &command[3], strlen(command));
                    args[strlen(command) - 3] = '\0';
                    write_file(args);
                    end_program = 1;
                }
                else if(startsWith("w ", command) != 0)
                {
                    memcpy(args, &command[2], strlen(command));
                    args[strlen(command) - 2] = '\0';
                    write_file(args);
                }

                else if(startsWith("r ", command) != 0)
                {
                    memcpy(args, &command[2], strlen(command));
                    args[strlen(command) - 2] = '\0';
                    read_file(args);
                    display_map(map);
                }
                else if(startsWith("n ", command) != 0)
                {
                    memcpy(args, &command[2], strlen(command));
                    args[strlen(command) - 2] = '\0';
                    new(args);
                    display_map(map);
                }
                else if(startsWith("a ", command) != 0)
                {
                    free(author);
                	author = malloc(sizeof(char) * (strlen(command) - 1));
                	memcpy(author, &command[2], strlen(command));
                	author[strlen(command) - 2] = '\0';
                }
                else if (startsWith("t ", command) != 0)
                {
                    free(title);
                	title = malloc(sizeof(char) * (strlen(command) - 1));
                	memcpy(title, &command[2], strlen(command));
                	title[strlen(command) - 2] = '\0';
                }
        }
    }

    /** disable display input characters on the screen */
	noecho();
}

void display_map(char* map)
{
	move(0, 0);

    attrset(COLOR_PAIR(3));

	for (int i = 0; i < height * width; i++)
	{
		if (i != 0 && i % width == 0)
		{
			printw("\n");
		}

		switch (map[i])
    	{
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
                attrset(COLOR_PAIR(1));
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
                attrset(COLOR_PAIR(6));
            	addch(ACS_PI);
                attrset(COLOR_PAIR(3));
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

    move(height, 0);

    for (int i = 0; i < width; i++)
    {
        addch(ACS_S1);
    }
    move(height + 6,0);
    clrtobot();
    mvprintw(height + 6,0,"The current author is: %s", author);
    mvprintw(height + 7,0,"The current map title is: %s", title);

    attrset(COLOR_PAIR(1));

	move(x, y);
}

void edit_mode(int input)
{
	switch(input)
    {
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

    move(x, y);
}

void new(char* args)
{	
    int temp_height;
    int temp_width;
    int count = 0;
    int error = 0;
    char *token = NULL;

    token = strtok(args, " ");
    while (token != NULL) 
    {
        if (count == 0)
        {   
        	reti = regcomp(&regex,"^[[:alnum:][:punct:]]",0);
          reti = regexec(&regex,token,0, NULL, 0);
			    if (reti == REG_NOMATCH)
			    {
          attrset(COLOR_PAIR(2));
				  mvprintw(height + 5, 0, "Please specify the file name correctly");
				  error = 1;
          attrset(COLOR_PAIR(1));
				  break;	
          }
          regfree(&regex);
          file_name = malloc(sizeof(char) * (strlen(token) + 1));
 			    memcpy(file_name, &token[0], strlen(token));
 			    file_name[strlen(token)] = '\0';
        } 
        else if (count == 1)
        {
          reti = regcomp(&regex,"^[[:digit:]]",0);
			    reti = regexec(&regex,token,0, NULL, 0);
			    
          if (reti == REG_NOMATCH)
			    {
          attrset(COLOR_PAIR(2));
				  mvprintw(height + 5, 0, "Please specify the number of rows correctly");
				  error = 1;
          attrset(COLOR_PAIR(1));
          break;
          }

          regfree(&regex);
          sscanf(token, "%d", &temp_height);
        } 
        else if (count == 2)
        {
        	reti = regcomp(&regex,"^[[:digit:]]",0);
			    reti = regexec(&regex,token,0, NULL, 0);
			    
          if (reti == REG_NOMATCH)
			    {
            attrset(COLOR_PAIR(2));
				    mvprintw(height + 5, 0, "Please specify the number of column correctly");
				    error = 1;
            attrset(COLOR_PAIR(1));
				    break;
          }

          regfree(&regex);
          sscanf(token, "%d", &temp_width);

        } else {
        	break; 
        }

        token = strtok(NULL, " ");
        count++;
    }
    

    if (error == 0)
    {
   		map = create_map(temp_height, temp_width);
   		clear();
	  }
    else
    {
        error_msg_count = 1;
    }
}


void write_file(char* file)
{	
	reti = regcomp(&regex,"^[[:alnum:][:punct:]]",0);
	reti = regexec(&regex,file,0, NULL, 0);
	if (reti == REG_NOMATCH)
    {
      attrset(COLOR_PAIR(2));
		  mvprintw(height + 5, 0, "Please specify the file name to be save or use 'w' to save the file with default name");
      attrset(COLOR_PAIR(1));
      error_msg_count = 1;
    } 
    else 
    {
        char path[strlen(DIRECTORY) + strlen(file) + 1];
        strcpy(path, DIRECTORY);
        strcat(path, file);

        int i;
        FILE *f = fopen(path,"w");
        fprintf(f, "%s\n", author);
        fprintf(f, "%s\n%i\n%i\n", title, height, width);

        for (i = 0; i < height * width; i++){
            if (i % width == 0 && i != 0)
                fprintf(f,"\n");
            fprintf(f,"%c",map[i]);
        }

        fprintf(f, "\n");

        fclose(f);
	}
	regfree(&regex);
}

void read_file(char* file)
{
	
	reti = regcomp(&regex,"^[[:alnum:][:punct:]]",0);
	reti = regexec(&regex,file,0, NULL, 0);
	if (reti == REG_NOMATCH) {
    attrset(COLOR_PAIR(2));
		mvprintw(height + 5, 0, "Please specify the file name to be open");
    attrset(COLOR_PAIR(1));
    error_msg_count = 1;
	}

  else 
	{
		int temp_height;
		int temp_width;

    size_t len;
    char *temp = NULL;
    char *temp_author = NULL;
    char *temp_title = NULL;
    char c;

    free(file_name);
    file_name = malloc(sizeof(char)*(strlen(file) + 1));
    memcpy(file_name, &file[0], strlen(file));
    file_name[strlen(file)] = '\0';

    char path[strlen(DIRECTORY) + strlen(file) + 1];
    strcpy(path, DIRECTORY);
    strcat(path, file);

    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
      attrset(COLOR_PAIR(2));
    	mvprintw(height + 5, 0,"No such file found");
      attrset(COLOR_PAIR(1));
      error_msg_count = 1;
      return;
    }
                
    getline(&temp_author, &len, f);
    getline(&temp_title, &len, f);

    free(author);
    author = malloc(sizeof(char) * (strlen(temp_author)));
 		memcpy(author, &temp_author[0], strlen(temp_author) - 1);
 		author[strlen(temp_author) - 1] = '\0';

    free(title);
 		title = malloc(sizeof(char) * (strlen(temp_title)));
 		memcpy(title, &temp_title[0], strlen(temp_title) - 1);
 		title[strlen(temp_title) - 1] = '\0';

    getline(&temp, &len, f);
    sscanf(temp, "%d", &temp_height);

    getline(&temp, &len, f);
    sscanf(temp, "%d", &temp_width);

    	map = create_map(temp_height, temp_width);

    	for (int row = 0; row < temp_height; row++)
    	{
        	getline(&temp, &len, f);

        	for (int col = 0; col < temp_width; col++)
        	{
            c = temp[col];
            map[row * width + col] = temp[col];
        }
    }

    free(temp);
    free(temp_author);
    free(temp_title);

    fclose(f);
    clear();
	}
}

char *create_map(int new_height, int new_width)
{
	
    height = new_height;
    width = new_width;

    if(new_map)
    {
		free(new_map);
	}
	new_map = malloc(sizeof(char) * (height * width));

	for (int i = 0; i < (height * width); i++)
	{
		new_map[i] = ' ';
	}

	for (int row = 1; row < height - 1; row++)
	{
		new_map[row * width] = 'a';
		new_map[row * width + width - 1] = 'd';
	}

	for (int col = 1; col < width - 1; col++)
	{
		new_map[col] = 'w';
		new_map[width * (height - 1) + col] = 'x';
	}

	new_map[0] = 'q';
	new_map[width - 1] = 'e';
	new_map[(height - 1) * width] = 'z';
	new_map[height * width - 1] = 'c';

	return new_map;
}

int startsWith(const char *pre, const char *str)
{
  size_t lenpre = strlen(pre),
  lenstr = strlen(str);
  return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}
