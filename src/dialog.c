#include "types.h"
#include "dialog.h"

WINDOW *create_dialog_window(const char *title) {
  WINDOW *win;
  int width = 40, height = 20;
  int sx, sy, i, u, startpos;
  // create a little window in the center of the screen with a border and a size of 40x20
  getmaxyx(stdscr, sy, sx);
  win = newwin(height, width, sy / 2 - height / 2, sx / 2 - width / 2);
  // make the border
  wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');

  // fill the first two lines with "-" and "+" on the edge
  for(u = 1; u <= 2; u++) {
    for(i = 0; i < width; i++) {
      mvwprintw(win, u, i, "%c", i == 0 || i == width - 1 ? '+' : '-');
    }
  }

  // print the title in the middle of the dialog
  startpos = width / 2 - strlen(title) / 2;
  mvwprintw(win, 1, startpos - 1, " ");
  mvwprintw(win, 1, startpos, "%s", title);
  mvwprintw(win, 1, startpos + strlen(title), " ");
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

int create_enter_dialog(const char *title, const char *contents[], int lines) {
  WINDOW *win = create_dialog_window(title);
  int i;
  // insert stuff into the dialog
  for(i = 0; i < lines; i++) {
    mvwprintw(win, i + 3, 1, "%s", contents[i]);
  }
  wrefresh(win);
  wait_return(win);
  delwin(win);
  return 0;
}

#define MENU_LINES 4

int display_menu() {
  WINDOW *win;
  int i, ch;
  // the contents of the dialog
  char menu[MENU_LINES][41] = {
  "%i) Start the game",
  "%i) Controls",
  "%i) Help",
  "%i) Exit"
  };
  
  // get a new dialog window
  win = create_dialog_window("MENU");

  // insert stuff into the dialog
  for(i = 1; i <= MENU_LINES; i++) {
    mvwprintw(win, i + 2, 1, menu[i - 1], i);
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

void display_controls() {
  // the dialog contents
  char controls[][41] = {
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

  // create the dialog
  create_enter_dialog("CONTROLS", (const char **)controls, 11);
}

#define HELP_LINES 11
void display_help() {
  WINDOW *win;
  int i;
  // the contents of the dialog
  char help[HELP_LINES][41] = {
  " < ^ > v  the snake head",
  " O        the snake body",
  "          you shouldn't hit yourself  ",
  " #        the wall, don't hit it!",
  " x        simple food, +1 highscore",
  "          +1 length, +1 new food",
  " @        double food, +1 highscore",
  "          +1 length, +2 new food",
  " %        mega food, +10 highscore",
  "          +5 length, +1 new food",
  "press enter to go back to the menu .."
  };

  // get a new dialog window
  win = create_dialog_window("HELP");

  for(i = 1; i <= HELP_LINES; i++) {
    mvwprintw(win, i + 2, 1, "%s", help[i - 1]);
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
  win = create_dialog_window("GAME OVER");

  // print stuff into the dialog
  mvwprintw(win, 3, 1, "highscore : %i", game->highscore);
  mvwprintw(win, 4, 1, "time      : %.0lfs", difftime(game->ended, game->started));
  mvwprintw(win, 6, 1, "press enter key to continue ");
  wrefresh(win);

  // wait until enter is pressed
  wait_return(win);
  delwin(win);
}


#define PAUSE_LINES 2

int pause_dialog() {
  WINDOW *win;
  int i, ch;
  // the contents of the dialog
  char dialog[PAUSE_LINES][41] = {
  "%i) Resume",
  "%i) Exit"
  };
  
  // get a new dialog window
  win = create_dialog_window("PAUSE");

  // insert stuff into the dialog
  for(i = 1; i <= PAUSE_LINES; i++) {
    mvwprintw(win, i + 2, 1, dialog[i - 1], i - 2);
  }
  // display the dialog
  wrefresh(win);

  // wait for some input
  while((ch = wgetch(win))) {
    if(ch == ERR) continue;
    if(ch == '\n') ch = '0';
    if(ch >= '0' && ch <= '9') {
      // return the number pressed
      return ch - '0';
    }
  }
  return 0;

}


