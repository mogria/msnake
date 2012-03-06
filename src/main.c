#include <stdlib.h>
#include "types.h"

int display_menu();

int main() {
  int action;
  // for some better random numbers
  srand(time(NULL));
  
  // start the curses mode
  initscr();
  //getch returns ERR if no input is present and doesn't wait
  nodelay(stdscr, TRUE);
  //don't echo the inserted keys to the screen
  noecho();
  //also grab keys like F1 etc.
  keypad(stdscr, TRUE);

  do {
    // display the menu
    action = display_menu();
    if(action == 1) {
      run();
    }

  } while(action != 2);

  // end the curses mode
  endwin();
  
  // close the logfile
  glog(NULL);
  return EXIT_SUCCESS;
}

#define MENU_ENTRIES 2
#define MENU_LINES (MENU_ENTRIES + 2)


int display_menu() {
  WINDOW *win;
  int sx, sy, ch, i;
  char menu[MENU_LINES][41] = {
  "---------------- MENU ----------------",
  "--------------------------------------",
  "1) Start the game",
  "2) Exit"
  };
  getmaxyx(stdscr, sy, sx);
  win = newwin(20, 40, sy / 2 - 10, sx / 2 - 20);
  wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');

  for(i = 1; i <= MENU_LINES; i++) {
    mvwprintw(win, i, 1, "%s", menu[i - 1]);
  }
  wrefresh(win);

  while(ch = wgetch(win)) {
    if(ch == ERR) continue;
    if(ch >= '0' && ch <= '9') {
      return ch - '0';
    }
  }
  return 0;
}

void display_highscore(GAME *game) {
  int sx, sy, ch; 
  WINDOW *win;
  getmaxyx(stdscr, sy, sx);
  win = newwin(20, 40, sy / 2 - 10, sx / 2 - 20);
  wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwprintw(win, 1, 1, "------------- GAME  OVER -------------");
  mvwprintw(win, 2, 1, "--------------------------------------");
  mvwprintw(win, 3, 1, "highscore : %i", game->highscore);
  mvwprintw(win, 4, 1, "time      : %.0lfs", difftime(game->ended, game->started));
  mvwprintw(win, 6, 1, "press enter key to continue ");
  wrefresh(win);
  while(ch = wgetch(win)) {
    if(ch == '\n') {
      break;
    } 
  }
  delwin(win);
}



