#include "main.h"
#include "game.h"

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
      // run the game
      run();
    } else if(action == 2) {
      // display a dialog which explains the controls of the game
      display_controls();
    }
    // leave if the menu entry "exit" is chosen
  } while(action != 3);

  // end the curses mode
  endwin();
  
  // close the logfile
  glog(NULL);
  return EXIT_SUCCESS;
}

#define MENU_LINES (3 + 2)

WINDOW *create_dialog() {
  WINDOW *win;
  int sx, sy;
  // create a little window in the center of the screen with a border and a size of 40x20
  getmaxyx(stdscr, sy, sx);
  win = newwin(20, 40, sy / 2 - 10, sx / 2 - 20);
  wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
  return win;
}

void wait_return(WINDOW *win) {
  int ch;
  while((ch = wgetch(win))) {
    if(ch == '\n') {
      break;
    } 
  }
}

int display_menu() {
  WINDOW *win;
  int i, ch;
  // the contents of the dialog
  char menu[MENU_LINES][41] = {
  "---------------- MENU ----------------",
  "--------------------------------------",
  "%i) Start the game",
  "%i) Controls",
  "%i) Exit"
  };
  
  // get a new dialog window
  win = create_dialog();

  // insert stuff into the dialog
  for(i = 1; i <= MENU_LINES; i++) {
    mvwprintw(win, i, 1, menu[i - 1], i - 2);
  }
  // display the dialog
  wrefresh(win);

  // wait for some input
  while((ch = wgetch(win))) {
    if(ch == ERR) continue;
    if(ch >= '0' && ch <= '9') {
      // return the number pressed
      return ch - '0';
    }
  }
  return 0;
}


#define CONTROLS_ENTRIES 12
#define CONTROLS_LINES (CONTROLS_ENTRIES + 2)

void display_controls() {
  WINDOW *win;
  int i;
  // the contents of the dialog
  char controls[CONTROLS_LINES][41] = {
  "-------------- CONTROLS --------------",
  "--------------------------------------",
  "w - move up",
  "a - move left",
  "s - move down",
  "d - move right",
  "you can use the arrow keys too",
  "",
  "8 - slower",
  "9 - faster",
  "0 - reset speed",
  "p - pause the game"
  "",
  "press enter to go back to the menu .."
  };

  // get a new dialog window
  win = create_dialog();

  for(i = 1; i <= CONTROLS_LINES; i++) {
    mvwprintw(win, i, 1, "%s", controls[i - 1]);
  }
  //display the dialog
  wrefresh(win);

  // wait until enter is pressed
  wait_return(win);
  delwin(win);
}

void display_highscore(GAME *game) {
  WINDOW *win;
  // create a dialog
  win = create_dialog();

  // print stuff into the dialog
  mvwprintw(win, 1, 1, "------------- GAME  OVER -------------");
  mvwprintw(win, 2, 1, "--------------------------------------");
  mvwprintw(win, 3, 1, "highscore : %i", game->highscore);
  mvwprintw(win, 4, 1, "time      : %.0lfs", difftime(game->ended, game->started));
  mvwprintw(win, 6, 1, "press enter key to continue ");
  wrefresh(win);

  // wait until enter is pressed
  wait_return(win);
  delwin(win);
}


