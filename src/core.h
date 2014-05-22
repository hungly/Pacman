/** The size of ncruses terminal in characters of height and width */
extern struct winsize w;

extern struct pacghost pacman;
extern struct pacghost * ghost;

/** Pointer to array of char which indicates the current map */
extern char *map;

/** An integer value which indicates the distance (in rows) between the top of terminal and the first row of the map */
extern int x_offset;

/** An integer value which indicates the distance (in columns) between the left most of terminal and the first column of the map */
extern int y_offset;

/** An integer value which indicates the height of map */
extern int height;

/** An integer value which indicates the width of map */
extern int width;

/** An integer value which indicates the current score */
extern int score;

extern int live;

extern int level;

extern int totalPellet;

extern int atePellet;

extern int is_pacman_powered_up;

extern int counter;

extern char *title;

extern char *author;

extern char *file_name;

extern char *directory;

extern int difficulty;

extern int ghost_counter[4];

/**
  * Search for pacman spawn point and set the pacman position at that point.
  *
  * @param map the read map from file with the 'p' character indicates pacman spawn point
  * @param pacman pointer to the current position and direction of the pacman in the map
  *
  * @return the 1 if success found a spawn point, 0 otherwise
  */
int search_pacman(char * map, struct pacghost * pacman);

/**
  * Search for ghost spawn point and set the ghost position at that point.
  *
  * @param map the read map from file with the 'p' character indicates pacman spawn point
  * @param pacman pointer to the current position and direction of the pacman in the map
  * @param ghost pointer to the current position and direction of the ghost in the map
  * @param ghost_number the index of the ghost in the ghost's array
  *
  * @return the 1 if success found a spawn point, 0 otherwise
  */
int search_ghost(char * map, struct pacghost * ghost, const int ghost_number);

/**
  * Display pacman and all the ghosts on the screen based on it position and other game's status
  *
  * @param pacman pointer to the current position and direction of the pacman in the map
  * @param ghost pointer to the current position and direction of the ghost in the map
  * @param is_pacman_powered_up indicates the pacman status
  * @param counter indicate time left for the pacman powered up
  * @param ghost_counter[] array store the counter indicate the cool down time for each ghost
  */
void display_characters(struct pacghost * pacman, struct pacghost * ghost, const int is_pacman_powered_up, int counter, int ghost_counter[]);

/**
  * Clear pacman and all the ghosts on the screen based on it position
  *
  * @param pacman pointer to the current position and direction of the pacman in the map
  * @param ghost pointer to the current position and direction of the ghost in the map
  */
void delete_characters(struct pacghost * pacman, struct pacghost * ghost);

/**
  * Count the total number of pellet in the map.
  *
  * @param map the read map from file with the 'p' character indicates pacman spawn point
  * @param pellet pointer to the variable that stor the total number of pellet
  */
void count_pellet(char * map, int * pellet);

/**
  * Decrease the counters which indicate the time left for pacman powered up or ghost cool down time
  *
  * @param counter indicate time left for the pacman powered up
  * @param ghost_counter[] array store the counter indicate the cool down time for each ghost
  */
void count_down(int * counter, int ghost_counter[]);

/**
  * Save the current game progress
  *
  * @param file_name indicate the file name to be saved
  */
void save_game(char * file_name);

/**
  * Display the list of save file is /save directory 
  */
void display_save_list();

/**
  * Load the current game
  *
  * @param file indicate the file name to be loaded
  *
  * @return 1 if file exist, else 0
  */
int load_game(char * file);

/**
  * Clear unneccessary characters on the map
  */
void delete_unneed_char();

/**
  * Handle the pause menu
  *
  * @param is_pacman_powered_up indicate whether pacman has eaten power pellet. 1 if true else 0
  * @param counter indicate the countdown of powered up
  * @param live indicate the number of live
  */
int handle_pause_menu(int * is_pacman_powered_up, int * counter, int * live);

/**
  * Resume the game after the pause/save/load progress
  */
void resume_pause_game();

/**
  * Display a dialog that allow user to enter name.
  */
void end_game_dialog();

/**
  * Record name and score.
  * 
  * @param name is the name that user inputted
  */
void record_player_result(char * name);

/**
  * Show the game credit.
  */
void credit_game();