#include "dijkstra.h"
#include "lyquochung.h"
#include "movement.h"
#include "pacghost.h"
#include "level_editor.h"
#include <curses.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h> 

// map used by AI to determined the relative position of node from others
int * ai_map;

// node position mapping
int * nodes_position;

// cost from 1 node to others
int ** nodes_cost;

// way to go from one node to other
int ** routing_table;

// number of node in the current map
int num_nodes;

// where pacman will appear
int pacman_spawn_point_x;
int pacman_spawn_point_y;
int pacman_direction = -1;

void initialise_hung_ai() {
    // create AI's map
    ai_map = malloc(sizeof(int) * (height * width));

    // initialise AI's map data
    for (int i = 0; i < height * width; i++) {
        switch (map[i]) {
            case ' ':
            case 's':
            case 'S':
            case 'f':
            case 'F':
            case 'g':
            case 'G':
            case 'p':
            case 'P':
                // movable cells
                ai_map[i] = -3;
                break;
            default:
                // unmovable cells
                ai_map[i] = -2;
                break;
        }
    }

    // search for pacman spawn point and find 
    search_pacman_spawn_point(&pacman_spawn_point_x, &pacman_spawn_point_y);
    auto_find_path(pacman_spawn_point_x, pacman_spawn_point_y, ' ');

    // number of node in the map
    num_nodes = 0;

    // scan for all cells in the map                         
    for (int i = 0; i < height * width; i++) {
        if (ai_map[i] == -1) {
        // count the number of movable cell around a cell
            switch (count_available_move_cell(i / width, i % width)) {
                case 2:
                // there are only two movvable cell arround the current cell
                    if (is_corner_cell(i / width, i % width)) {
                        // is a corner then it is a node
                        ai_map[i] = num_nodes;
                        num_nodes++;
                    }
                    break;
                default:
                // otherwise it also a node
                    ai_map[i] = num_nodes;
                    num_nodes++;
                    break;
            }
        }
    }

    // create node's cost table for every nodes
    nodes_cost = malloc(sizeof(int *) * num_nodes);

    // initialise data for node's cost table
    for (int i = 0; i < num_nodes; i++) {
        nodes_cost[i] = malloc(sizeof(int) * num_nodes);
        for (int j = 0; j < num_nodes; j++)
            nodes_cost[i][j] = 0;
    }

    // create routing table for every nodes
    routing_table = malloc(sizeof(int *) * num_nodes);

    // initialise data for routing table
    for (int i = 0; i < num_nodes; i++)
        routing_table[i] = malloc(sizeof(int) * num_nodes);
    
    // initialise values for routing table
    for (int i = 0; i < num_nodes; i++)
        for (int j = 0; j < num_nodes; j++)
            if (i == j)
                routing_table[i][j] = i;
            else
                routing_table[i][j] = -1;

    // create a table which store a position of each node
    nodes_position = malloc(sizeof(int) * num_nodes);

    for (int i = 0; i < height * width; i++) {
        if (ai_map[i] >= 0) {
            // store node's position
            nodes_position[ai_map[i]] = i;

            // calculate cost from this node to other node
            // go in 4 directions
            for (int direction = 0; direction < 4; direction++) {
                int end_nodes_postion = i;
                end_nodes_postion = get_end_node(i, direction);
                if (i != end_nodes_postion)
                    calculate_cost(i, end_nodes_postion, direction);
            }
        }
    }

    // calculate routing table from prepaired data
    // code adapted from: http://rosettacode.org/wiki/Dijkstra's_algorithm#C
    dijkstra(num_nodes, nodes_cost, routing_table);
}

int count_available_move_cell(const int x, const int y) {
    int count = 0;

    // check for the above cell
    if ((x - 1) >= 0 && isValidMoveCell(x - 1, y))
        count++;
    // check for the right cell
    if ((x + 1) < height && isValidMoveCell(x + 1, y))
        count++;
    // check for the below cell
    if ((y - 1) >= 0 && isValidMoveCell(x, y - 1))
        count++;
    // check for the left cell
    if ((y + 1) < width && isValidMoveCell(x, y + 1))
        count++;

    return count;
}

void finish_hung_ai() {
    // free the Ai's map
    if(ai_map)
        free(ai_map);
    // free the node's cost table
    if (nodes_cost) {
        for (int i = 0; i < num_nodes; i++) {
            if (nodes_cost[i])
                free(nodes_cost[i]);
        }
        free(nodes_cost);
    }
    // free the routing table
    if (routing_table) {
        for (int i = 0; i < num_nodes; i++) {
            if (routing_table[i])
                free(routing_table[i]);
        }
        free(routing_table);
    }
    // free the node's position table
    if (nodes_position)
        free(nodes_position);
}

int is_corner_cell(const int x, const int y) {
    // check if the movable cell is oppssed each other or not
    if ((x - 1) >= 0 && isValidMoveCell(x - 1, y) && (x + 1) < height && isValidMoveCell(x + 1, y))
        return 0;
    else if ((y - 1) >= 0 && isValidMoveCell(x, y - 1) && (y + 1) < width && isValidMoveCell(x, y + 1))
        return 0;
    else
        return 1;
}

void auto_find_path(const int x, const int y, const char direction) {
    // fill the movable cell with -1
    switch (ai_map[(x) * width + y]) {
        case -3:
            ai_map[(x) * width + y] = -1;
            break;
        default:
            return;
    }

    /* recursively call itself in other cell after fill it with 's' if it contains ' ' */

    if (isValidFillCell(x + 1, y) && direction != 'u')
        auto_find_path(x + 1, y, 'd');
    if (isValidFillCell(x, y + 1) && direction != 'l')
        auto_find_path(x, y + 1, 'r');
    if (isValidFillCell(x - 1, y) && direction != 'd')
        auto_find_path(x - 1, y, 'u');
    if (isValidFillCell(x, y - 1) && direction != 'r')
        auto_find_path(x, y - 1, 'l');
}

int get_end_node(int current_node, const int direction) {
    // initialise variables
    int new_node;
    int end_function = 0;

    // run untill end signal
    while (!end_function) {
        // choose direction
        switch (direction) {
            case 0:
                new_node = current_node - width;

                // search untill meet a node
                // not a node
                if (new_node >= 0 && ai_map[new_node] == -1)
                    current_node = new_node;
                else if (ai_map[new_node] >= 0) {
                    current_node = new_node;
                    end_function = 1;
                } else
                    end_function = 1;
                break;
            case 1:
                new_node = current_node + 1;

                // search untill meet a node
                if (new_node % width > current_node % width && ai_map[new_node] == -1)
                    // not a node
                    current_node = new_node;
                else if (ai_map[new_node] >= 0) {
                    // if current cell is a node
                    current_node = new_node;
                    end_function = 1;
                } else
                    end_function = 1;
                break;
            case 2:
                new_node = current_node + width;

                // search untill meet a node
                if (new_node < height * width && ai_map[new_node] == -1)
                    // not a node
                    current_node = new_node;
                else if (ai_map[new_node] >= 0) {
                    // if current cell is a node
                    current_node = new_node;
                    end_function = 1;
                } else
                    end_function = 1;
                break;
            case 3:
                new_node = current_node - 1;

                // search untill meet a node
                if (new_node % width < current_node % width && ai_map[new_node] == -1)
                    // not a node
                    current_node = new_node;
                else if (ai_map[new_node] >= 0) {
                    // if current cell is a node
                    current_node = new_node;
                    end_function = 1;
                } else
                    end_function = 1;
                break;
            default:
                end_function = 1;
                break;
        }
    }

    // return the node
    return current_node;
}

void calculate_cost(const int start_node_position, const int end_nodes_postion, const int direction) {
    // choose direction
    switch (direction) {
        case 0:
            nodes_cost[ai_map[start_node_position]][ai_map[end_nodes_postion]] = (start_node_position / width) - (end_nodes_postion / width);
            break;
        case 1:
            nodes_cost[ai_map[start_node_position]][ai_map[end_nodes_postion]] = end_nodes_postion - start_node_position;
            break;
        case 2:
            nodes_cost[ai_map[start_node_position]][ai_map[end_nodes_postion]] = (end_nodes_postion / width) - (start_node_position / width);
            break;
        case 3:
            nodes_cost[ai_map[start_node_position]][ai_map[end_nodes_postion]] = start_node_position - end_nodes_postion;
            break;
        default:
            break;
    }
}

void hung_ai(char * map, struct pacghost * pacman, struct pacghost * ghost, const int difficulty, const int is_pacman_powered_up) {
    if (is_pacman_powered_up)
        // hunt the pacman
        run_away(pacman, ghost);
    else
        // run away
        hunt(pacman, ghost, difficulty);
}

int find_nearest_node(struct pacghost * character) {
    // initialise variables
    int current_position = character->xLocation * width + character->yLocation;
    int end_nodes_postion;
    int nearest_node = current_position;
    int cost = 9999;
    int temp = 0;

    for (int direction = 0; direction < 4; direction++) {
        end_nodes_postion = current_position;
        // get the next node in the given direction
        end_nodes_postion = get_end_node(current_position, direction);
        if (current_position != end_nodes_postion) {
            // choose direction
            switch (direction) {
                case 0:
                    temp = (current_position / width) - (end_nodes_postion / width);
                    break;
                case 1:
                    temp = end_nodes_postion - current_position;
                    break;
                case 2:
                    temp = (end_nodes_postion / width) - (current_position / width);
                    break;
                case 3:
                    temp = current_position - end_nodes_postion;
                    break;
                default:
                    break;
            }

            // if the temp cost is less than the current cost then this is the new nearest node
            if (temp < cost) {
                cost = temp;
                nearest_node = end_nodes_postion;
            }
        }
    }

    // return the nearest node
    return ai_map[nearest_node];
}

void move_to_node(const int current_position, const int destination_node, struct pacghost * ghost, struct pacghost * pacman) {
    // if ghost at it current destination
    if (current_position == nodes_position[destination_node]) {
        if (ghost->xLocation == pacman->xLocation) {
            // starting node and destination ndoe in the same row
            if (ghost->yLocation - pacman->yLocation > 0)
                // move left
                ghost->direction = 3;
            else
                // move right
                ghost->direction = 1;
        } else if (ghost->yLocation == pacman->yLocation) {
            // starting node and destination ndoe in the same column
            if (ghost->xLocation - pacman->xLocation > 0)
                // move up
                ghost->direction = 0;
            else
                // move down
                ghost->direction = 2;
        }
    }
    // if it is not reach its destination and it is not currently moving
    else {
        int offset = nodes_position[destination_node] - current_position;
        if (offset > 0) {
            if (offset < width)
                // move right
                ghost->direction = 1;
            else
                // move down
                ghost->direction = 2;
        } else if (offset < 0) {
            offset = offset * -1;
            if (offset < width)
                // move left
                ghost->direction = 3;
            else
                // move up
                ghost->direction = 0;
        } else
            // do not move
            ghost->direction = 4;
    }
}

int get_destination_node(int pacman_position, struct pacghost * pacman) {
    // initialise destination node
    int destination_node = pacman_position;

    if (ai_map[pacman_position] < 0) {
        // loop until find a node following the direction
        do {
            switch (pacman->direction) {
                case 0:
                    destination_node -= width;
                    break;
                case 1:
                    destination_node += 1;
                    break;
                case 2:
                    destination_node += width;
                    break;
                case 3:
                    destination_node -= 1;
                    break;
                default:
                // pacman does not move
                    destination_node = nodes_position[find_nearest_node(pacman)];
                    break;
            }
        } while(ai_map[destination_node] < 0);
    }

    return ai_map[destination_node];
}

void hunt(struct pacghost * pacman, struct pacghost * ghost, const int difficulty) {
    int ghost_position = ghost->xLocation * width + ghost->yLocation;
    int pacman_position = pacman->xLocation * width + pacman->yLocation;

    // predict which node pacman will move to
    int pacman_destination_node = get_destination_node(pacman_position, pacman);

    if (pacman_direction != pacman->direction && ai_map[pacman_position] >= 0 && ai_map[ghost_position] >= 0 && difficulty >= 2) {
        // pacman just change its direction
        pacman_direction = pacman->direction;
        move_to_node(ghost_position, routing_table[ai_map[ghost_position]][pacman_destination_node], ghost, pacman);
        // mvprintw(0, 0, "pacman change direction");
        return;
    } else if (ai_map[pacman_position] >= 0 && ai_map[ghost_position] >= 0 && difficulty >= 1) {
        move_to_node(ghost_position, routing_table[ai_map[ghost_position]][pacman_destination_node], ghost, pacman);
        // mvprintw(0, 0, "pacman over a node     ");
        return;
    }
    // if ghost is over a node
    else if (ai_map[ghost_position] >= 0) {
        move_to_node(ghost_position, routing_table[ai_map[ghost_position]][pacman_destination_node], ghost, pacman);
        return;
    }
    // ghost is not over a node
    else if (ghost->direction == 4) {
        // try to move to the nearest node
        int ghost_nearest_node = find_nearest_node(ghost);
        move_to_node(ghost_position, ghost_nearest_node, ghost, pacman);
        return;
    }
}

void run_away(struct pacghost * pacman, struct pacghost * ghost) {
    static int ghost_destination = 0;

    int ghost_position = ghost->xLocation * width + ghost->yLocation;

    // if there is no destination picked yet or already arrived at current destination then pick a new one
    if (ghost_destination == 0 || ai_map[ghost_position] == ghost_destination)
        ghost_destination = random_node(num_nodes, pacman);
    
    // if ghost still on the move
    if (ai_map[ghost_position] >= 0) {
        // ghost arrived at its destination
        if (ai_map[ghost_position] == ghost_destination)
            // choose new destination
            random_node(num_nodes, pacman);
        // move to the next node in the routing table
        move_to_node(ghost_position, routing_table[ai_map[ghost_position]][ghost_destination], ghost, pacman);
    }
    // ghost is not over a node
    else if (ghost->direction == 4) {
        int ghost_nearest_node = find_nearest_node(ghost);
        move_to_node(ghost_position, ghost_nearest_node, ghost, pacman);
    }
}

int random_node(const int max, struct pacghost * pacman) {
    static int Init = 0;

    int do_again = 1;

    int pacman_position = pacman->xLocation * width + pacman->yLocation;

    int pacman_destination_node = get_destination_node(pacman_position, pacman);

    if (Init == 0) {
        srand(time(NULL));
        Init = 1;
    }

    int destination_node = (rand() % max);

    // loop until a suitable node is picked
    while (do_again) {
        // random node is not the pacman destination node
        if (destination_node == pacman_destination_node)
            destination_node = (rand() % max);
        else {
            for (int i = 0; i < num_nodes; i++) {
                if (destination_node == routing_table[pacman_destination_node][i]) {
                    // random node is one of the node that can move to from the pacman destination node according to the routing table
                    // pick another node
                    destination_node = (rand() % max);
                    do_again = 1;
                    break;
                } else
                    // done
                    do_again = 0;
            }
        }
    }
    
    return (destination_node);
}