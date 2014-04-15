#include "core.h"
#include "pacghost.h"
#include <curses.h>
#include <sys/ioctl.h>

void search_pacman(char * map, struct pacghost * pacman) {
    for (int i = 0; i < height * width; i++) {
        if (map[i] == 'p' || map[i] == 'P') {
            pacman->xLocation = i / width;
            pacman->yLocation = i % width;
            break;
        }
    }
}

void search_ghost(char * map, struct pacghost * ghost) {
    int ghost_index = 0;
    int i = 0;
    while (true) {
        if (map[i] == 'g' || map[i] == 'G') {
            ghost[ghost_index].xLocation = i / width;
            ghost[ghost_index].yLocation = i % width;   
            
            ghost_index++;
            if (ghost_index == 4) {
                break;
            }
        }
        i++;
        if (i == height * width)
        {
            i = 0;
        }
    }
}

void display_characters(struct pacghost * pacman, struct pacghost * ghost) {
    mvprintw(pacman->xLocation + x_offset + 4, pacman->yLocation + y_offset, "O");
    for (int i = 0; i < 4; i++)
    {
        mvprintw(ghost[i].xLocation + x_offset + 4, ghost[i].yLocation + y_offset, "W");
    }

    move(w.ws_row - 1, w.ws_col - 1);
}