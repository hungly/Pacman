/** The size of ncruses terminal in characters of height and width */
extern struct winsize w;

extern int score;

extern int live;

extern int level;

extern char * map;

extern int height;

extern int width;

extern struct pacghost pacman;

/**
  * Display the new score on the screen.
  */
void display_score();

/**
  * Update the score of the player.
  *
  * @param map pointer to the current map in the memory
  * @param height current height of the map
  * @param width current width of the map
  * @param pacman pointer to the current position and direction of the pacman in the map
  * @param currentScore pointer to the player's score
  * @param atePellet pointer to the number of pellet that been eaten
  * @param is_pacman_powered_up pointer to the integer indicate the status of the pacman
  * @param counter pointer to the integer which store time counter for paman powered up
  */
void update_score(char * map, int height, int width, struct pacghost * pacman, int * currentScore, int * atePelet, int * is_pacman_powered_up, int * counter);

/**
 * Check if the level is completed or not.
 *
 * @param atePelet indicates the number of pellet pacman has eaten
 * @param totalPellet indicates the total number of available pellet in the current loaded map
 *
 * @return 1 if all pellet has been eaten, 0 otherwise
 */

int isWin(int atePelet, int totalPelet);

/**
  * Get the high score from file and display it. The file must be named as "result.rec"
  * and save with the following format: one line for player, then another for score. 
  *
  * @param argc number of argument  
  * @param argv pointer to the array of char which indicate the argument itself
  */
void display_high_score(int argc, char *argv[]);