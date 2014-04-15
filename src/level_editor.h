#include <regex.h>

int editor(int argc, char *argv[]);

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
void  replace_marked_point(const char original, const char replacement);

/** Pointer to array of char which indicates the current map */
extern char *map;

/** Pointer to array to char which indicates the author of current map */
extern char *author;

/** Pointer to array to char which indicates the title of current map */
extern char *title;

/** Pointer to array to char which indicates the file name of current map */
extern char *file_name;

/** Pointer to array to char which indicates the present working directory */
extern char *directory;

/** The size of ncruses terminal in characters of height and width */
extern struct winsize w;

/** An integer value which indicate the program is ended or not */
extern int end_program;

/** An integer value which indicate the number of error message */
extern int error_msg_count;

/** An integer value which indicates the height of map */
extern int height;

/** An integer value which indicates the width of map */
extern int width;

/** An integer value which indicates the row where the cursor is located in the map */
extern int x;

/** An integer value which indicates the column where the cursor is located in the map */
extern int y;

/** An integer value which indicates the distance (in rows) between the top of terminal and the first row of the map */
extern int x_offset;

/** An integer value which indicates the distance (in columns) between the left most of terminal and the first column of the map */
extern int y_offset;

/** regex is used to  check for input pattern */
extern regex_t regex;

/** reti is an integer which store the result of regex comparision */
extern int reti;