/** The size of ncruses terminal in characters of height and width */
extern struct winsize w;

extern int score;

extern int live;

extern int level;

extern char * map;

extern int height;

extern int width;

void display_score();

void updateScore(char * map, int height, int width, struct pacghost * pacman, int currentScore );