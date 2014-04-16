/** The size of ncruses terminal in characters of height and width */
extern struct winsize w;

extern int score;

extern int live;

extern int level;

extern char * map;

extern int height;

extern int width;

extern struct pacghost pacman;

void display_score();

void update_score(char * map, int height, int width, struct pacghost * pacman, int * currentScore, int * atePelet );

int isWin(int atePelet, int totalPelet);