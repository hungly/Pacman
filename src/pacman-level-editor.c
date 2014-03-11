#include <stdlib.h>
#include <string.h>
#include <curses.h>

#define DEFAULT_HEIGHT 10
#define DEFAULT_WIDTH 30
#define DIRECTORY "../levels/"

void command_mode();
void display_map(char* map);
void edit_mode(int input);
void new(char* args);
void write_file(char* file);
void read_file(char* file);
void finish();

char *create_map(int height, int width);
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

int main()
{
	int input;

	author = "Hung Ly Quoc <s3426511@rmit.edu.vn>, Liem Ly Quan <s3426110@rmit.edu.vn>";
	title = "Default title";
	height = 15;
	width = 40;

  // initialise curses
	initscr();
  // enable the use of function keys, allow navigating the cursor using arrow keys
	keypad(stdscr, TRUE);
  // disable echo when getch
	noecho(); 
  // take input chars, does not wait until new line or carriage return
	cbreak();

	map = create_map(height, width);

  // run until program is ended
	while(!end_program)
	{
  	// display current map from the memory
		display_map(map);

    // get input command

		input = getch();

    // check for ":" to enter command mode
		if (input == ':')

		{
			command_mode();
		}
		else
		{
			edit_mode(input);
		}
	}
	
	finish();
}


/**
* VI liked command mode for map editor.
* This function will check if the input command is either q, w, n, r or wq and performed the following actions:
* q: quit the program and exit to the terminal command prompt.
* w: save the current map to the levels directory, this command accepts a parameter as the saved file name, if there is no file name, the map will be saved using the default name "pacman.pac".
* wq: save and then quit the program, this command perform the same actions as the "w" command and then the "q" command.
* n: create a new map, this command take three parameters, the first parameter is the new map name, the second and third parameters specify the height and width of the new map.
* r: load a map from the levels directory into the map editor, this command accept one parameter specify the file name of the map.
*/
void command_mode()
{
	char args[1000];

	int count = 0;
	int input;

	char command[1000];

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

	command[count] = '\0';

    if (input == 10)
    {
        

        switch (strlen(command)){
        	// check for user input in command mode
            case 1:
                if(strcmp(command, "q") == 0)
                {
                    end_program = 1;
                    return;
                } else if (strcmp(command,"w") == 0) {
                    write_file("level.pac");
                    clrtoeol();
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
                    clrtoeol();
                }
                else if(startsWith("r ", command) != 0)
                {
                    memcpy(args, &command[2], strlen(command));
                    args[strlen(command) - 2] = '\0';
                    read_file(args);
                    clear();
                    display_map(map);
                }
                else if(startsWith("n ", command) != 0)
                {
                    memcpy(args, &command[2], strlen(command));
                    args[strlen(command) - 2] = '\0';
                    new(args);
                    clear();
                    display_map(map);
                }
                else if(startsWith("a ", command) != 0)
                {
                	author = malloc(sizeof(char) * (strlen(command) - 1));
                	memcpy(author, &command[2], strlen(command));
                	author[strlen(command) - 2] = '\0';
                	clrtoeol();
                }
                else if (startsWith("t ", command) != 0)
                {
                	title = malloc(sizeof(char) * (strlen(command) - 1));
                	memcpy(title, &command[2], strlen(command));
                	title[strlen(command) - 2] = '\0';
                	clrtoeol();
                }
                else
                {
                    clrtoeol();
                }
                break;
        }
    }

	noecho();
}

void display_map(char* map)
{
	// method for drawing a map using an array
	move(0, 0);

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
    	        addch(ACS_BULLET);
        	    break;
        	case 'S':
            	addch(ACS_DEGREE);
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
            	addch(ACS_PI);
            	break;
        	case 'p':
        	case 'P':
            	addch(ACS_STERLING);
            	break;
        	case 'f':
        	case 'F':
	            addch(ACS_DIAMOND);
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
    move(height + 5,0);
    clrtobot();
    mvprintw(height + 5,0,"The current author is: %s", author);
    mvprintw(height + 6,0,"The current map title is: %s", title);


	move(x, y);
}

void edit_mode(int input)
{
	// method for handling in editing map mode 
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
	// method for hadling new map creation
    int temp_height;
    int temp_width;
    int count = 0;
    char *token = NULL;

    token = strtok(args, " ");
    while(token != NULL) {
        switch(count)
        {
            case 0:
                file_name = token;
                break;
            case 1:
                sscanf(token, "%d", &temp_height);
                break;
            case 2:
                sscanf(token, "%d", &temp_width);
                break;
            default:
                break;
        }
        token = strtok(NULL, " ");
        count++;
    }

    if (map)
    {
        free(map);
    }
    map = create_map(temp_height, temp_width);
}

void finish()
{
	// method for ending program 
	endwin();


	if (author){
		free(author);
	}
	if (title){
		free(title);
	}
	if (map){
		free(map);
	}
    

	exit(0);
}

void write_file(char* file)
{
	
	// method for writing map information to file
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

void read_file(char* file)
{
<<<<<<< HEAD
	/* method for reading map information from file */
	int temp_height;
	int temp_width;

=======
>>>>>>> bab34717ab8665d40d9a8e6c7b686cead8d34b2e
    size_t len;
    char *temp = NULL;
    char c;

    char path[strlen(DIRECTORY) + strlen(file) + 1];
    strcpy(path, DIRECTORY);
    strcat(path, file);

    FILE *f = fopen(path, "r");

    if(f == NULL)
        return;

    getline(&author, &len, f);
    getline(&title, &len, f);

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

    fclose(f);
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
	/** 
	 * Function for checking the beginning of the string 
	 * reference:
	 * http://stackoverflow.com/questions/4770985/something-like-startswithstr-a-str-b-in-c
	 */
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}
