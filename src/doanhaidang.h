/**
 * Check to see if pacman can move to the cell
 *
 * @param x The horizontal location of the cell
 * @param y The vertical location of the cell
 * @return 0 is true, 1 is false
 */
int is_valid_cell(int x, int y);

/**
 * Keep adding valid cells
 *
 * @param x The horizontal location
 * @param y The vertical location
 * @param direction The direction which the process scans
 */
void add_cell(int x, int y, char direction);

/**
 * This call add_cell to collect all valid cells
 *
 * @param pacman_x The starting horizontal location of pacman
 * @param pacman_y The starting vertical location of pacman
 */
void collect_valid_cell(int pacman_x, int pacman_y);

/**
 * Initialize everything needed for later functions
 *
 * @param pacman_x The starting horizontal location of pacman
 * @param pacman_y The starting vertical location of pacman
 */
void init_dang_ghost(int pacman_x, int pacman_y);

/**
 * Breadth First Search all paths
 *
 * @param x The horizontal location of ghost
 * @param y The vertical location of ghost
 */
void bfs(int x, int y);

/**
 * Instruct ghost to move based on pacman's location
 *
 * @param map The map's information
 * @param pacman The pacman's information
 * @param ghost The ghost's information
 * @param difficulty The game's difficulty
 * @param is_pacman_powered_up Whether pacman eats power pellet
 */
void move_dang_ghost(char * map,struct pacghost *pacman, struct pacghost *ghost,const int difficulty, int is_pacman_powered_up);

/**
 * Release memory on heap
 */
void finish_dang_ghost();
