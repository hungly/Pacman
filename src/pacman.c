#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>

// define your own colors: the numbers correspond to the colors defined
// in the terminal configuration
#define COLOR_BACKGROUND 0
#define COLOR_WALL       1
#define COLOR_PACMAN     2

/**
  * Based on an example taken on this site:
  * http://invisible-island.net/ncurses/ncurses-intro.html
  *
  * @author http://invisible-island.net/ncurses/ncurses-intro.html
  * @author Denis Rinfret
  */
static void finish(int sig);

int main(int argc, char *argv[])
{
    int num = 0;
    char out[75];
    struct timespec delay = {0, 500000000L}, 
                     rem;
    
    //delay.tv_sec = 0;
    //delay.tv_nsec = 500000000L;
    
    /* initialize your non-curses data structures here */
    
    (void) signal(SIGINT, finish);      /* arrange interrupts to terminate */

    (void) initscr();      /* initialize the curses library */
    keypad(stdscr, TRUE);  /* enable keyboard mapping */
    (void) nonl();         /* tell curses not to do NL->CR/NL on output */
    (void) cbreak();       /* take input chars one at a time, no wait for \n */
    (void) noecho();         /* echo input - in color */

    if (has_colors()) {
    
        start_color();
        // in theory, you can change the value of the 8 predefined colors
        // but it works only some times
        //assert(init_color(COLOR_YELLOW, 200, 200, 1000) == OK);
        // instead, you have to redefine the colors in the terminal

        // initialise you colors pairs (foreground, background)
        init_pair(1, COLOR_WALL,    COLOR_BACKGROUND);
        init_pair(2, COLOR_PACMAN,  COLOR_BACKGROUND);

    }
    
    // add a bunch of characters
    // you should probably use the function mvaddch and other similar function
    // the move the cursor to some position
    attrset(COLOR_PAIR(1));
    addch(ACS_ULCORNER);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_TTEE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_TTEE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_URCORNER);
    addch('\n');
    addch(ACS_VLINE);
    addch(' ');
    addch(ACS_CKBOARD);
    addch(' ');
    addch(ACS_VLINE);
    addch(' ');
    addch(' ');
    addch(' ');
    addch(ACS_VLINE);
    addch(' ');
    addch(' ');
    attrset(COLOR_PAIR(2));
    addch(ACS_DIAMOND);
    attrset(COLOR_PAIR(1));
    addch(' ');
    addch(ACS_VLINE);
    addch('\n');
    addch(ACS_LTEE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(' ');
    addch(ACS_VLINE);
    addch(' ');
    addch(ACS_ULCORNER);
    addch(ACS_HLINE);
    addch(ACS_RTEE);
    addch(' ');
    addch(ACS_ULCORNER);
    addch(ACS_URCORNER);
    addch(' ');
    addch(ACS_VLINE);
    addch('\n');
    addch(ACS_ULCORNER);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_URCORNER);
    addch('\n');
    addch(ACS_LLCORNER);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_LRCORNER);
    addch('\n');
    
    // print the hexadecimal values of some of these extended characters
    sprintf(out, "%x %x %x %x %x", (int)ACS_ULCORNER, (int)ACS_LLCORNER, (int)ACS_URCORNER, (int)ACS_LRCORNER,(int)A_ALTCHARSET);
    addstr(out);
    sprintf(out, "   %c %c %c %c %c", (char)(ACS_ULCORNER^A_ALTCHARSET), (char)(ACS_LLCORNER^A_ALTCHARSET), (char)(ACS_URCORNER^A_ALTCHARSET), (int)(ACS_LRCORNER^A_ALTCHARSET), (char)(A_ALTCHARSET^A_ALTCHARSET));
    addstr(out);
    
    for (;;) {
        int c = getch();     /* refresh, accept single keystroke of input */
        /* process the command keystroke */
        if (c == 'q') {
            break;
        }
        sprintf(out, "%i", c);
        addstr(out);
        if (c == KEY_DOWN) {
            addstr("down");
        }
        
	    attrset(COLOR_PAIR(num % 8));
	    num++;
	    
	    // sleep 
        nanosleep(&delay, &rem);
        
        
    }

    finish(0);               /* we're done */
}

static void finish(int sig)
{
    endwin();

    /* do your non-curses wrapup here */

    exit(0);
}
