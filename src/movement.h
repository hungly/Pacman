//#include "pacghost.h"

/** Pointer to array of char which indicates the current map */
extern char *map;

/** An integer value which indicates the height of map */
extern int height;

/** An integer value which indicates the width of map */
extern int width;

int isCollision(struct pacghost * pacman, struct pacghost * ghost);

void move_character(struct pacghost * character);

int isValidMoveCell(const int current_x, const int current_y);

int canMove(const struct pacghost * character);