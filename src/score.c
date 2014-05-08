#include "score.h"
#include "pacghost.h"
#include <curses.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include "level_editor.h"

/** Define the directory when opening using sh file in root folder of project*/
#define DIRECTORY_SH            "save/"

/** Define the directory when opening using the executable file in source folder*/
#define DIRECTORY_SRC           "../save/"

void display_score() {
	attrset(COLOR_PAIR(8));
	mvprintw(w.ws_row - 2, 7, "%d", score);
	mvprintw(w.ws_row - 1, 7, "%d", live);
	mvprintw(w.ws_row - 2, w.ws_col / 4 * 3 + 15, "%d", level);
	attrset(COLOR_PAIR(3));
}

void update_score(char * map, int height, int width, struct pacghost * pacman, int * currentScore, int * atePelet, int * is_pacman_powered_up, int * counter) {
	//pelet +10 points
	//fruit +50 points
	if(map[width * pacman[0].xLocation + pacman[0].yLocation] == 's') {
		currentScore[0] = currentScore[0] + 10;
		map[width * pacman[0].xLocation + pacman[0].yLocation] = ' ';
		(*atePelet)++;
	}
	if(map[width * pacman[0].xLocation + pacman[0].yLocation] == 'S') {
		map[width * pacman[0].xLocation + pacman[0].yLocation] = ' ';
		if (*counter != -1) {
			(*is_pacman_powered_up) = 1;
			(*counter) = 50;
		}
	}
	if(map[width * pacman[0].xLocation + pacman[0].yLocation] == 'f' || map[width * pacman[0].xLocation + pacman[0].yLocation] == 'F' || map[width * pacman[0].xLocation + pacman[0].yLocation] == 'S' ) {
		currentScore[0] = currentScore[0] + 50;
		map[width * pacman[0].xLocation + pacman[0].yLocation] = ' ';
	}
}

int isWin(int atePelet, int totalPelet) {
	if(atePelet == totalPelet)
		return 1;
	else
		return 0;
}

void display_high_score(int argc, char * argv[]) {
	int y_offset = w.ws_col / 2;
	/* Count number of line http://stackoverflow.com/questions/1910724/c-retrieving-total-line-numbers-in-a-file */
	size_t len = 0;
	char *temp = NULL;
	int ch;
	int line = 0;
	int temp_score;
	char *temp_name = NULL;
	char * file = "result.rec";
	char * directory = NULL;
	
	/* check for directory first */
	if (startsWith("./src/", argv[0])) 
        directory = DIRECTORY_SH;
    else 
        directory = DIRECTORY_SRC;
	char path[strlen(directory) + strlen(file)];
    strcpy(path, directory);
    strcat(path, file);
	FILE *f = fopen(path,"r");
	
	/* if no record file */
	if (f == NULL){
		mvprintw(30, 0,"No record file found. Press any key to back to menu");
		getch();
		return;
	}

	/* Read the first time to get the number of lines */
	while ((ch = fgetc(f)) != EOF)
		if (ch == '\n')
			line++;

	/* Back to beginning of file */
	rewind(f);

	/* Initilise array based on number of line */
	char ** player = malloc(sizeof(char*)*(line/2));
	int *score = malloc(sizeof(int)*line/2);

	/* Read the second time */
	for (int i = 0; i < line/2; i++){
		int length = 0;
		length = getline(&temp,&len, f);
		player[i] = malloc(sizeof(char)*length);
		strcpy(player[i], temp);
		getline(&temp,&len, f);
		sscanf(temp,"%d", &score[i]);
	}
	
	/* After all data are got from the file, close the file */
	fclose(f);

	/* Sort the data */
	for (int i = 0; i < line / 2; i++){
		for (int j = i+1; j < line / 2; j++){
			if (score[i] < score[j]){
				temp_score = score[i];
				score[i] = score[j];
				score[j] = temp_score;
				temp_name = player[i];
				player[i] = player[j];
				player[j] = temp_name;
			}
		}
	}

	/* If we have less than 10 record, print all, else, print 10 only */
	if (line < 20){
		mvprintw(8, y_offset - 7, "%s","Top high score");
		for (int i = 0; i < line / 2;i++){
			mvprintw(10 + i, y_offset - 21, "%20s" , player[i]);
			mvprintw(10 + i, y_offset, "|  %i" , score[i]);
		}
	} else {
		mvprintw(8, y_offset - 8, "%s", "Top 10 high score"); 
		for (int i = 0; i < 10;i++){
			mvprintw(10 + i, y_offset - 21, "%20s" , player[i]);
			mvprintw(10 + i, y_offset, "|  %i" , score[i]);
		}
	}

	/* Free player, temp, and score */
	for (int i = 0; i < line / 2; i++)
		free(player[i]);
	free(player);
	free(temp);
	free(score);

	mvprintw(10 +  (line / 2 + 2), y_offset - 18, "%s", "Press any key to return to main menu");
	getch();
}
