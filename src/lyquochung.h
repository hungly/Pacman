/** The size of ncruses terminal in characters of height and width */
extern struct winsize w;

/** The pacman's position in the map and its current direction */
extern struct pacghost pacman;

/**
  * Read the current map in the memory then construct a special map (AI's map (ai_map)) and a routing table based on that map.
  * The map and routing table will be used to instruct the AI to chase and run away from the pacman.
  */
void initialise_hung_ai();

/**
  * Count the number of movable cell around a given node on the map. The number range from 0 to maximum of 4.
  *
  * @param x the row number in the map of the given node
  * @param y the column number in the map of the given node
  *
  * @return the number of moveable cell surround the given node
  */
int count_available_move_cell(const int x, const int y);

/**
  * Clear all the memory used by the AI
  */
void finish_hung_ai();

/**
  * Check if the given cell in the map is a corner or not.
  * Corner is the cell which has two movable cells around it and their position is not opposed each other.
  * i.e.: the two movable cells should not be one above and one below/one on the right and one the left of the given cell.
  *
  * @param x the row number in the map of the given node
  * @param y the column number in the map of the given node
  *
  * @return 0 if it is not a corner cell, 1 otherwise
  */
int is_corner_cell(const int x, const int y);

/**
  * Automatically filling all the appropriate places (movable cells) with a number.
  */
void auto_find_path(const int x, const int y, const char direction);

/**
  * Get the position of the node that directly connected to the given node in the given direction.
  *
  * @param current_node the position of the node that need to get the node which connected to it
  * @param direction the direction that need to be check for (0 is up, 1 is right, 2 is down, 3 is left)
  *
  * @return the position of the node which directly connected to the given node in the given direction
  */
int get_end_node(int current_node, const int direction);

/**
  * Calculate the cost need to go from a node to another node in the given direction.
  *
  * @param start_node_position the position of the starting node
  * @param end_node_position the position of the destination node
  * @param direction the direction to go to get to destination node from the starting node (0 is up, 1 is right, 2 is down, 3 is left)
  */
void calculate_cost(const int start_node_position, const int end_nodes_postion, const int direction);

/**
  * Main function for the AI, instruct the ghost the move in the appropriate direction based on the provided in formation.
  *
  * @param map the current map in the memory
  * @param pacman pointer to the current position and direction of the pacman in the map
  * @param ghost pointer to the current position and direction of the ghost which this AI function control
  * @param difficulty the game difficulty setting
  * @param is_pacman_powered_up the integer indicate the pacman's status
  */
void hung_ai(char * map, struct pacghost * pacman, struct pacghost * ghost, const int difficulty, const int is_pacman_powered_up);

/**
  * Find the nearest node from the AI's map the closest to the given game character (pacman or ghost)
  *
  * @param character the game character (pacman or ghost) which need to find it closest node
  *
  * @return the position of the closest node to the given game character
  */
int find_nearest_node(struct pacghost * character);

/**
  * Instruct the ghost to move from on node to other node based on the given information.
  *
  * @param current_postion the current position of the ghost on the map
  * @param destination_node the node which the ghost need to move to
  * @param pacman pointer to the current position and direction of the pacman in the map
  * @param ghost pointer to the current position and direction of the ghost which this AI function control
  */
void move_to_node(const int current_position, const int destination_node, struct pacghost * ghost, struct pacghost * pacman);

/**
  * Predict the node which the pacman is most likely try to move to based on the given information
  *
  * @param pacman_position the position of the pacman in the 
  * @param pacman pointer to the current position and direction of the pacman in the map
  *
  * @return the node which is the destination of the pacman
  */
int get_destination_node(int pacman_position, struct pacghost * pacman);

/**
  * Try to hunt for the pacman, predict it movement and intercept it.
  *
  * @param pacman pointer to the current position and direction of the pacman in the map
  * @param ghost pointer to the current position and direction of the ghost which this AI function control
  * @param difficulty the game difficulty setting
  */
void hunt(struct pacghost * pacman, struct pacghost * ghost, const int difficulty);

/**
  * Run away from th pacman if it can kill the ghost.
  *
  * @param pacman pointer to the current position and direction of the pacman in the map
  * @param ghost pointer to the current position and direction of the ghost which this AI function control
  */
void run_away(struct pacghost * pacman, struct pacghost * ghost);

/**
  * Randomly pick a node in the list which satisfy a set of conditions to set as a destination for a ghost to run away from pacman.
  *
  * @param pacman pointer to the current position and direction of the pacman in the map
  */
int random_node(const int max, struct pacghost * pacman);