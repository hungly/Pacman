//extern struct winsize w;

int is_valid_cell(int x, int y);

void add_cell(int x, int y, char direction);

void collect_valid_cell(int pacman_x, int pacman_y);

void init_dang_ghost(int pacman_x, int pacman_y);

void bfs(int x, int y);

void move_dang_ghost(struct pacghost *ghost, struct pacghost *pacman, int powered_up);

void finish_dang_ghost();