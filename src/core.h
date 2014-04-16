/** The size of ncruses terminal in characters of height and width */
extern struct winsize w;

extern struct pacghost pacman;
extern struct pacghost * ghost;

/** An integer value which indicates the distance (in rows) between the top of terminal and the first row of the map */
extern int x_offset;

/** An integer value which indicates the distance (in columns) between the left most of terminal and the first column of the map */
extern int y_offset;

/** An integer value which indicates the height of map */
extern int height;

/** An integer value which indicates the width of map */
extern int width;

void search_pacman(char * map, struct pacghost * pacman);

void search_ghost(char * map, struct pacghost * ghost);

void display_characters(struct pacghost * pacman, struct pacghost * ghost);

void delete_characters(struct pacghost * pacman, struct pacghost * ghost);