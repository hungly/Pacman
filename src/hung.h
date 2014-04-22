extern struct winsize w;

extern struct pacghost pacman;

void initialise_hung_ai(const char * map, const int height, const int width);

int count_available_move_cell(const int x, const int y);

void display_ai_map();

void display_cost_table();

void finish_hung_ai();

int is_corner_cell(const int x, const int y);

void auto_find_path(const int x, const int y, const char direction);

void calculate_cost(const int i, int * end, int * cost, const int j);