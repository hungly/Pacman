/** Pointer to array of char which indicates the current map */
extern char *map;

/** An integer value which indicates the height of map */
extern int height;

/** An integer value which indicates the width of map */
extern int width;

/**
 * Check if the pacman and ghost has collide or not.
 *
 * @param pacman pointer to the current position and direction of the pacman in the map
 * @param ghost pointer to the current position and direction of the ghost in the map
 *
 * @return 1 if pacman and ghost is collided, 0 otherwise
 */
int isCollision(struct pacghost * pacman, struct pacghost * ghost);

/**
 * Change a given game's character position based on its direction.
 *
 * @param character pointer to the position and direction of the character that position need to be changed
 */
void move_character(struct pacghost * character);

/**
 * Check if a given cell in the map is movable for pacman or ghost.
 *
 * @param current_x the row of the cell in the map
 * @param current_y the column of the cell in the map
 *
 * @return 1 if the cell is movable, 0 otherwise
 */
int isValidMoveCell(const int current_x, const int current_y);

/**
 * Check if a given game's character can move on to the next cell based on its direction or not.
 *
 * @param character pointer to the position and direction of the character that position need to be changed
 *
 * @return 1 if the the given game's character can move on, 0 otherwise
 */

int canMove(const struct pacghost * character);