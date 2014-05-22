#include "core.h"
#include "pacghost.h"
#include <curses.h>
#include <sys/ioctl.h>
#include "level_editor.h"
#include <dirent.h> 
#include <stdlib.h>
#include <string.h>
#include "score.h"
#include "lyquochung.h"
#include "lyquanliem.h"
#include "doanhaidang.h"

/** Define the directory when opening using sh file in root folder of project*/
#define DIRECTORY_SH            "save/"

/** Define the directory when opening using the executable file in source folder*/
#define DIRECTORY_SRC           "../save/"

/** cheat sequence for god mode */
int god_sequence[] = {KEY_UP, KEY_UP, KEY_DOWN, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT, 'b', 'a'};

/** the length of all cheat sequences */
int sequence_length = 10;

/** number of correct key strike for god cheat */
int current_god_sequence_index = 0;

/** 8:11 AM cheat sequence for bonus lives */
int live_sequence[] = {KEY_UP, KEY_UP, KEY_DOWN, KEY_DOWN, KEY_LEFT, KEY_LEFT, KEY_RIGHT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT};

/** number of correct key strike for lives cheat */
int current_live_sequence_index = 0;

int search_pacman(char * map, struct pacghost * pacman) {
    // search through all cell of the map
    for (int i = 0; i < height * width; i++) {
        // meet the first spawn point
        if (map[i] == 'p' || map[i] == 'P') {
            pacman->xLocation = i / width;
            pacman->yLocation = i % width;
            return 1;
        }
    }
    // no spawn point found
    return 0;
}

int search_ghost(char * map, struct pacghost * ghost, const int ghost_number) {
    int ghost_index = 0;
    int i = 0;
    while (1) {
        // meet a spawn point for ghost
        if (map[i] == 'g' || map[i] == 'G') { 
            // if the current ghost index is match with the ghost number then set the ghost position and exit
            if (ghost_index == ghost_number) {
                ghost->xLocation = i / width;
                ghost->yLocation = i % width;
                ghost->direction = 4;
                return 1;
                break;
            } else
                ghost_index++;
        }
        i++;
        if (i == height * width) {
            // reach the end of the map and found no spawn point
            if (ghost_index == 0) {
                return 0;
                break;
            } else
                i = 0;
        }
    }
}

void display_characters(struct pacghost * pacman, struct pacghost * ghost, const int is_pacman_powered_up, int counter, int ghost_counter[]) {
    // display ghost
    for (int i = 0; i < 4; i++) {   
        if (ghost_counter[i] == 0) {
            // set color based on time left
            if (is_pacman_powered_up)
                if (counter > 30 || counter == -1)
                    attrset(COLOR_PAIR(7));
                else
                    attrset(COLOR_PAIR(5));
            else
                attrset(COLOR_PAIR(4 + i));

            mvprintw(ghost[i].xLocation + x_offset + 4, ghost[i].yLocation + y_offset, "W");
        } else {
            // circle ghost color
            attrset(COLOR_PAIR(4 + i));
            mvprintw(ghost[i].xLocation + x_offset + 4, ghost[i].yLocation + y_offset, ":");
        }
    }

    // display pacman
    attrset(COLOR_PAIR(4));
    mvprintw(pacman->xLocation + x_offset + 4, pacman->yLocation + y_offset, "O");
}

void delete_characters(struct pacghost * pacman, struct pacghost * ghost) {
    // delete old pacman character
    attrset(COLOR_PAIR(3));
    mvprintw(pacman->xLocation + x_offset + 4, pacman->yLocation + y_offset, " ");

    // delete old ghost characters
    for (int i = 0; i < 4; i++) {
        move(ghost[i].xLocation + x_offset + 4, ghost[i].yLocation + y_offset);
        switch (map[ghost[i].xLocation * width + ghost[i].yLocation]) {
            case 's':
                attrset(COLOR_PAIR(6));
                addch(ACS_BULLET);
                attrset(COLOR_PAIR(3));
                break;
            case 'S':
                attrset(COLOR_PAIR(7));
                addch(ACS_DEGREE);
                attrset(COLOR_PAIR(3));
                break;
            case 'f':
            case 'F':
                attrset(COLOR_PAIR(5));
                addch(ACS_DIAMOND);
                attrset(COLOR_PAIR(3));
                break;
            default:
                addch(' ');
                break;
        }
    }
}

void count_pellet(char * map, int * pellet) {
    int i = 0;

    *pellet = 0;

    // search through map in the memory
    for (i = 0; i < height * width; i++)
        if(map[i] == 's')
            (*pellet)++;
}

void count_down(int * counter, int ghost_counter[]) {
    // reduce counter for pacman status
    if (*counter > 0)
        (*counter)--;

    // reduce counter for ghost cool down
    for (int i = 0; i < 4; i++)
        if (ghost_counter[i] > 0)
            ghost_counter[i]--;
}

void save_game(char * file_name){

    // check for the path first
    char * dir = NULL;
    if (strcmp(directory, "levels/") == 0)
        dir = "save/";
    else
        dir = "../save/";

    char path[strlen(dir) + strlen(file_name)];
    strcpy(path, dir);
    strcat(path, file_name);
    
    // open the file for writing
    FILE *f = fopen(path,"w+");

    // write the author, title, file_name, height, width
    fprintf(f, "%s", author);
    fprintf(f, "\n%s", title);
    fprintf(f, "\n%s", file_name);
    fprintf(f, "\n%i", height);
    fprintf(f, "\n%i\n", width);

    // write the map
    for (int i = 0; i < height * width; i++)
        for (i = 0; i < height * width; i++) {
            if (i % width == 0 && i != 0)
                fprintf(f, "\n");
            fprintf(f, "%c", map[i]);
        }

    // write the position and direction of pacman and 4 ghosts
    fprintf(f, "\n%i", pacman.xLocation);
    fprintf(f, "\n%i", pacman.yLocation);
    fprintf(f, "\n%i", pacman.direction);
    
    for (int i = 0; i < 4; i++){
        fprintf(f, "\n%i", ghost[i].xLocation);
        fprintf(f, "\n%i", ghost[i].yLocation);
        fprintf(f, "\n%i", ghost[i].direction);
    }

    // write the score, live, level, total pellet, ate pellet, difficulty
    fprintf(f, "\n%i", score);
    fprintf(f, "\n%i", live);
    fprintf(f, "\n%i", level);
    fprintf(f, "\n%i", totalPellet);
    fprintf(f, "\n%i", atePellet);
    fprintf(f, "\n%i", difficulty);
    fprintf(f, "\n%i", is_pacman_powered_up);

    // if pacman is powered up, write down the countdown for powered up and ghost respawn
    if (is_pacman_powered_up) {
        fprintf(f, "\n%i", counter);
        fprintf(f, "\n%i", ghost_counter[0]);
        fprintf(f, "\n%i", ghost_counter[1]);
        fprintf(f, "\n%i", ghost_counter[2]);
        fprintf(f, "\n%i", ghost_counter[3]);
    }

    // close file
    fclose(f);
}

int load_game(char * file) {

    // check for path first
    char * dir = NULL;
    if (strcmp(directory, "levels/") == 0)
        dir = "save/";
    else
        dir = "../save/";

    char path[strlen(dir) + strlen(file)];
    strcpy(path, dir);
    strcat(path, file);

    // open the file
    FILE *f = fopen(path,"r");

    // if file not found, return 0
    if (f == NULL)
        return 0;

    // free the existing data first
    free_ghost_map();
    finish_hung_ai();
    finish_dang_ghost();

    clear();
    
    char *temp = NULL;
    char *temp_author = NULL;
    char *temp_title = NULL;
    char *temp_file_name = NULL;
    
    int temp_height;
    int temp_width;
    
    size_t len;
    
    getline(&temp_author, &len, f);
    getline(&temp_title, &len, f);
    getline(&temp_file_name,&len, f);

    /* update author */
    if (author)
        free(author);
    author = malloc(sizeof(char) * (strlen(temp_author)));
    memcpy(author, &temp_author[0], strlen(temp_author) - 1);
    author[strlen(temp_author) - 1] = '\0';


    /* update tittle */
    if (title)
        free(title);
    title = malloc(sizeof(char) * (strlen(temp_title)));
    memcpy(title, &temp_title[0], strlen(temp_title) - 1);
    title[strlen(temp_title) - 1] = '\0';

    /* update file name */
    if (file_name)
        free(file_name);
    file_name = malloc(sizeof(char) * (strlen(temp_file_name)));
    memcpy(file_name, &temp_file_name[0], strlen(temp_file_name) - 1);
    file_name[strlen(temp_file_name) - 1] = '\0';

    //  update height 
    getline(&temp, &len, f);
    sscanf(temp, "%d", &temp_height);

    // update width
    getline(&temp, &len, f);
    sscanf(temp, "%d", &temp_width);

    // update map
    map = create_map(temp_height, temp_width);
   
    for (int row = 0; row < temp_height; row++) {
            getline(&temp, &len, f);

            for (int col = 0; col < temp_width; col++)
                map[row * width + col] = temp[col];
    }

    // update pacman and ghost direction and location
    getline(&temp, &len, f);    
    pacman.xLocation = (int) strtol(temp, (char **)NULL, 10);
    getline(&temp, &len, f);
    pacman.yLocation = (int) strtol(temp, (char **)NULL, 10);
    getline(&temp, &len, f);
    pacman.direction = (int) strtol(temp, (char **)NULL, 10);

    for (int i = 0; i < 4; i++){
        getline(&temp, &len, f);
        ghost[i].xLocation = (int) strtol(temp, (char **)NULL, 10);
        getline(&temp, &len, f);
        ghost[i].yLocation = (int) strtol(temp, (char **)NULL, 10);
        getline(&temp, &len, f);
        ghost[i].direction = (int) strtol(temp, (char **)NULL, 10);
    }

    // update score, live, level, total pellet, ate pellet, difficulty, pacman powered up
    getline(&temp, &len, f);
    score = (int) strtol(temp, (char **)NULL, 10);

    getline(&temp, &len, f);
    live = (int) strtol(temp, (char **)NULL, 10);

    getline(&temp, &len, f);
    level = (int) strtol(temp, (char **) NULL, 10);

    getline(&temp, &len, f);
    totalPellet = (int) strtol(temp, (char **) NULL, 10);

    getline(&temp, &len, f);
    atePellet = (int) strtol(temp, (char **)NULL, 10);

    getline(&temp, &len, f);
    difficulty = (int) strtol(temp, (char **)NULL, 10);

    getline(&temp, &len, f);
    is_pacman_powered_up = (int) strtol(temp, (char **)NULL, 10);

    // if pacman is powered up, update pacman powered up and ghost counter
    if (is_pacman_powered_up) {
        getline(&temp, &len, f);
        counter = (int) strtol(temp, (char **)NULL, 10);
        getline(&temp, &len, f);
        ghost_counter[0] = (int) strtol(temp, (char **)NULL, 10);
        getline(&temp, &len, f);
        ghost_counter[1] = (int) strtol(temp, (char **)NULL, 10);
        getline(&temp, &len, f);
        ghost_counter[2] = (int) strtol(temp, (char **)NULL, 10);
        getline(&temp, &len, f);
        ghost_counter[3] = (int) strtol(temp, (char **)NULL, 10);

    }

    // close file
    fclose(f);
    free(temp);
    free(temp_title);
    free(temp_author);
    free(temp_file_name);

    // initiliase ai
    initialise_hung_ai();
    initialise_ghost_map();

    init_dang_ghost(pacman);
    return 1;
}

void display_save_list() {
   /*
    * This program displays the names of all files in the current directory.
    * Reference: http://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
    */
    DIR * d;
    struct dirent *dir;
    if (strcmp(directory, "levels/") == 0)
        d = opendir("save/");
    else
        d = opendir("../save/");

    int count = 0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (!startsWith(".",dir->d_name) && strcmp(dir->d_name,"result.rec")){
                mvprintw(18 + count, w.ws_col / 2 - 4,"%s", dir->d_name);
                count++;
            }
        }
        closedir(d);
    }
}

void delete_unneed_char(){
    attrset(COLOR_PAIR(3));
    /* calculate the offset to display the map on screen */
    x_offset = (((w.ws_row - 6) / 2) - (height / 2)) > 0 ? ((w.ws_row -6) / 2) - (height / 2): 0;
    y_offset = ((w.ws_col / 2) - (width / 2)) > 0 ? (w.ws_col / 2) - (width / 2): 0;
    for (int i = 0; i < height * width; ++i)
        if (map[i] == 'g' || map[i] == 'p')
            mvprintw(4 + x_offset + (i / width), (i % width) + y_offset, " ");
}

int handle_pause_menu(int * is_pacman_powered_up, int * counter, int * live){
    // show cursor
    curs_set(1);

    //set color
    attrset(COLOR_PAIR(9));
    
    y_offset = w.ws_col / 2;
    
    // wait for user input
    timeout(-1);

    int inputChoice;

    // show the input
    echo();


    while (true) {
        do {
            mvprintw(10, y_offset - 4, "%s", "Paused");
            mvprintw(11, y_offset - 9, "%s", "ESC. Resume game");
            mvprintw(12, y_offset - 7, "%s", "1. Save game");
            mvprintw(13, y_offset - 7, "%s", "2. Load game");
            mvprintw(14, y_offset - 10, "%s", "3. Quit to main menu");
            mvprintw(16, y_offset - 10, "%s", "Your choice is: ");
            inputChoice = getch();

            // check for god cheat sequences
            if (inputChoice == god_sequence[current_god_sequence_index]) {
                current_god_sequence_index++;
                if (current_god_sequence_index == sequence_length) {
                    if (*is_pacman_powered_up == 0 && *counter == 0) {
                        *is_pacman_powered_up = 1;
                        *counter = -1;
                    } else {
                        *is_pacman_powered_up = 0;
                        *counter = 0;
                    }
                    current_god_sequence_index = 0;
                }
            } else {
                current_god_sequence_index = 0;
            }

            // check for bonus live cheat sequences
            if (inputChoice == live_sequence[current_live_sequence_index]) {
                current_live_sequence_index++;
                if (current_live_sequence_index == sequence_length) {
                    *live+=30;
                    current_live_sequence_index = 0;
                }
            } else {
                current_live_sequence_index = 0;
            }
        } while (!(inputChoice == '1' || inputChoice == '2' || inputChoice == 27 || inputChoice == '3'));

        // if user press 3
        if (inputChoice == '3') {
            return 1;
        }

        if (inputChoice == 27) {
            resume_pause_game();
            break;
        }

        // if user press 1 or 2
        if (inputChoice  == '1' || inputChoice == '2') {
            if (inputChoice == '1')
                mvprintw(17, y_offset - 14,"%s", "Enter file name to save: ");
            else {
                display_save_list();
                mvprintw(17, y_offset - 14,"%s", "Enter file name to load: ");
            }
            char filename[100];
            int count = y_offset + 10;
            
            int input = getch();

            /* if pressed key is not "Enter" or "Escape" */
            while (input != 27 && input != 10) {

                /* move the cursor right 1 character */
                move(17, count + 1);

                /* if "Backspace" is pressed */
                if (input == KEY_BACKSPACE  && count > y_offset + 10) {
                    /* move cursor left 1 character and clear everything on the right */
                    move(17, count);
                    clrtoeol();
                    count--;

                    /* wait for another input */
                    input = getch();

                    /* run another iteration of the loop */
                    continue;
                }

                /* if key other than "Left", "Right", "Up", "Down" and "Backspace" is pressed */
                if (input != KEY_UP && input != KEY_DOWN && input != KEY_LEFT && input != KEY_RIGHT && input != KEY_BACKSPACE) {

                    /* move the cursor to the right 1 character */
                    move(17, count + 2);

                    /* save the input character to the memory */
                    filename[count - y_offset - 10] = input;

                    /* increase the file name length */
                    count++;
                }

                /* wait for another input */
                input = getch();       
            }
            /* if "Escape" is pressed */
            if (input == 27) {
                /* clear every input */
                move(16, 0);
                clrtobot();

            }   
            /* clear the filename string */
            filename[count - y_offset - 10] = '\0';

            /* if "Enter" is pressed */
            if (input == 10) {
                char args[100];
                memcpy(args, &filename[0], strlen(filename));
                args[strlen(filename)] = '\0';
                if (inputChoice == '1'){
                    save_game(args);
                    resume_pause_game();
                    break;
                } else {
                    if (load_game(args)){
                        resume_pause_game();
                        break;
                    } else {
                        move(16, 0);
                        clrtobot();
                        mvprintw(w.ws_row - 1, y_offset - 8, "File not found");
                    }
                }
            }

        }
    }

    // disable showing input
    noecho();

    // not show cursor
    curs_set(0);

    // do not wait for user input
    timeout(0);

    return 0;
}

void resume_pause_game() {
    clear();
    display_map(map);
    delete_unneed_char();
    display_score();
    attrset(COLOR_PAIR(9));
    mvprintw(w.ws_row - 2, 0, "Score: ");
    mvprintw(w.ws_row - 1, 0, "Live:  ");
    mvprintw(w.ws_row - 2, w.ws_col / 4 * 3 + 7, "Level: ");
}

void end_game_dialog() {
    
    attrset(COLOR_PAIR(9));
    
    // show cusor
    curs_set(1);
    clear();

    // show input
    echo();
    y_offset = w.ws_col / 2;
    mvprintw(16, y_offset - 7, "Please enter your name: ");
    char player_name[100];
    timeout(-1);
    move(17, y_offset+1);
    
    int count = y_offset;
            
    int input = getch();

    /* if pressed key is not "Enter" or "Escape" */
    while (input != 10) {

        /* move the cursor right 1 character */
        move(17, count + 1);

        /* if "Backspace" is pressed */
        if (input == KEY_BACKSPACE  && count != 32) {

            /* move cursor left 1 character and clear everything on the right */
            move(17, count);
            clrtoeol();
            count--;

            /* wait for another input */
            input = getch();

            /* run another iteration of the loop */
            continue;
        }

        /* if key other than "Left", "Right", "Up", "Down" and "Backspace" is pressed */
        if (input != KEY_UP && input != KEY_DOWN && input != KEY_LEFT && input != KEY_RIGHT && input != KEY_BACKSPACE) {

            /* move the cursor to the right 1 character */
            move(17, count + 2);

            /* save the input character to the memory */
            player_name[count - y_offset] = input;

            /* increase the file name length */
            count++;
        }

        /* wait for another input */
        input = getch();       
    }
 
    /* clear the player name string */
    player_name[count - y_offset] = '\0';

    /* if "Enter" is pressed */
    if (input == 10) {
        char args[100];
        memcpy(args, &player_name[0], strlen(player_name));
        args[strlen(player_name)] = '\0';
        record_player_result(player_name);
    }

    // hide cusor
    curs_set(0);
}


void record_player_result(char * name) {

    // save the player name and score in to result.rec
    char * dir = NULL;
    char * file = "result.rec";
    if (strcmp(directory, "levels/") == 0)
        dir = "save/";
    else
        dir = "../save/";

    char path[strlen(dir) + strlen(file)];
    strcpy(path, dir);
    strcat(path, file);
    FILE *f = fopen(path,"a");
    fprintf(f,"%s\n", name);
    fprintf(f,"%i\n", score);
    fclose(f);
}

void credit_game(){
    clear();
    mvprintw(3, y_offset - 21, " _____        _____ __  __          _   _ ");
    mvprintw(4, y_offset - 21, "|  __ \\ /\\   / ____|  \\/  |   /\\   | \\ | |");
    mvprintw(5, y_offset - 21, "| |__) /  \\ | |    | \\  / |  /  \\  |  \\| |");
    mvprintw(6, y_offset - 21, "|  ___/ /\\ \\| |    | |\\/| | / /\\ \\ | . ` |");
    mvprintw(7, y_offset - 21, "| |  / ____ \\ |____| |  | |/ ____ \\| |\\  |");
    mvprintw(8, y_offset - 21, "|_| /_/    \\_\\_____|_|  |_/_/    \\_\\_| \\_|");
    mvprintw(12, y_offset - 21, "               Game Credit                ");
    attrset(COLOR_PAIR(1));
    mvprintw(15, y_offset - 21, "         Ly Quan Liem - s3426110          ");
    mvprintw(16, y_offset - 21, "         \"Follower\" (White Ghost)         ");
    attrset(COLOR_PAIR(8));
    mvprintw(18, y_offset - 21, "         Ly Quoc Hung - s3426511          ");
    mvprintw(19, y_offset - 21, "         \"Hunter\" (Yellow Ghost)          ");
    attrset(COLOR_PAIR(9));
    mvprintw(21, y_offset - 21, "         Doan Hai Dang - s3425475         ");
    mvprintw(22, y_offset - 21, "         \"Warriors\" (Green Ghost)         ");
    attrset(COLOR_PAIR(10));
    mvprintw(24, y_offset - 21, "        Nguyen Vinh Linh - s3410595       ");
    mvprintw(25, y_offset - 21, "           \"Halo\" (Pink Ghost)            ");
    attrset(COLOR_PAIR(9));
    mvprintw(27, y_offset - 21, "         Ho Chi Minh - 07/05/2014        ");
    mvprintw(w.ws_row - 2, y_offset - 18,"Copyright (c) 2014 - Segmentation Fault\n");
    getch();

}