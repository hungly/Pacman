/** The size of ncruses terminal in characters of height and width */
extern struct winsize w;

extern struct pacghost * ghost;

extern struct pacghost pacman;

extern char *map;

/** An integer value which indicates the height of map */
extern int height;

/** An integer value which indicates the width of map */
extern int width;

/** Pointer to an array of char which indicates the current map with number of direction for ghost */
char *ghost_map;

/**
  * Initlialise the ghost map every time the level is changed.
  * The ghost map simply generate the number of move available ways of every valid cell
  */
void initialise_ghost_map();

/**
  * Move the ghost randomly. Except when it is moving
  * and it cannot turn left or right
  * 
  * @param ghost pointer to the ghost we want to move randomly
  */
void move_randomly(struct pacghost * ghost);

/**
  * Choose the direction of the ghost 
  * based on the difficulty and power pallette eaten 
  * 
  * @param map pointer to the array of char which indicate current map
  * @param pacman pointer to the pacman
  * @param pacghost pointer to a ghost we want to choose direction for it
  * @param diffculty an integer indicate the difficulty. 0 for easy and 3 for insane
  * @param is_pacman_powered_up an integer indicate whether pacman is powered up or not. 0 for no and 1 for yes
  */
void choose_direction_for_ghost(char * map, struct pacghost * pacman, struct pacghost * ghost, int difficulty, int is_pacman_powered_up);

/**
  * The direction that the ghost (with position xStart, yStart) turn to when move to
  * the destination (with xDestination, yDestination)
  *
  * The way direction found is based on the A-star algorithm with some modification to fit the program
  *
  * Reference: 
  * http://web.mit.edu/eranki/www/tutorials/search/
  *
  * @param xStart the row of start point 
  * @param yStart the column of start point
  * @param xDestination the row of destination point
  * @param yDestination the column of destination point
  * 
  * @return the direction that the ghost should turn to 
  */
int direction_from_node_to_node(int xStart, int yStart, int xDestination, int yDestination);

/**
  * This method move ghost to pacman position 
  *
  * @param pacman: pointer to the struct of pacman
  * @param ghost: pointer to the struct of the ghost
  */
void chase_after_pacman(struct pacghost * pacman, struct pacghost * ghost);

/**
  * Function for getting square of number 
  *
  * @param a is the float to get the squared of 
  *
  * @return the square of the float a
  */
float sqr(float a);

/**
  * Calculate the distance between two points start and destination
  *
  * @param xStart the row of start
  * @param yStart the column of start  
  * @param xDestination the row of destination
  * @param yDestination the column of destination
  * 
  * @return the distance between start and destination
  */
float distance_between_two_points(int xStart, int yStart, int xDestination, int yDestination);

/**
  * Free ghost map if it exists 
  */
void free_ghost_map();

/** A function to return a number in range [min,max)
  *
  * @param min the minimum number of range
  * @param max the maximum number (the maximum number can be randomed is max - 1 )
  */
int random_in_range (unsigned int min, unsigned int max);