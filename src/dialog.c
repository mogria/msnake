#include "types.h"
#include "highscore.h"
#include "dialog.h"

// create a basic dialog with title
WINDOW *create_dialog_window(const char *title) {
  WINDOW *win;
  int sx, sy, i, u, startpos;
  // create a little window in the center of the screen with a border and a size of 40x20
  getmaxyx(stdscr, sy, sx);
  win = newwin(DIALOG_HEIGHT, DIALOG_WIDTH, sy / 2 - DIALOG_HEIGHT / 2, sx / 2 - DIALOG_WIDTH / 2);
  // make the border
  wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');

  // fill the first two lines with "-" and "+" on the edge
  for(u = 1; u <= 2; u++) {
    for(i = 0; i < DIALOG_WIDTH; i++) {
      mvwprintw(win, u, i, "%c", i == 0 || i == DIALOG_WIDTH - 1 ? '+' : '-');
    }
  }

  // print the title in the middle of the dialog
  startpos = DIALOG_WIDTH / 2 - strlen(title) / 2;
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

// creates a simple dialog with content, you can press 'enter' to close the dialog
int create_enter_dialog(const char *title, const char *contents, int lines) {
  WINDOW *win = create_dialog_window(title);
  int i;
  // insert stuff into the dialog
  for(i = 0; i < lines; i++) {
    mvwprintw(win, i + 3, 1, "%s", &contents[i * CONTENT_WIDTH]);
  }
  // display the dialog
  wrefresh(win);
  // wait for 'enter'
  wait_return(win);
  // free the allocated memory of the window
  delwin(win);
  return 0;
}


// creates a dialog with menu entries, you can press numbers to select a menu entry and close the dialog
int create_numbered_dialog(const char *title, const char *contents, int lines) {
  WINDOW *win = create_dialog_window(title);
  int i, ch;
  // insert menu entries into
  for(i = 0; i < lines; i++) {
    mvwprintw(win, i + 3, 1, &contents[i * CONTENT_WIDTH], i + 1);
  }
  // display the dialog
  wrefresh(win);

  // wait for some input
  while((ch = wgetch(win))) {
    if(ch == ERR) continue;
    // select the first menu entry if enter is pressed
    if(ch == '\n') ch = '1';
    // a number pressed?
    if(ch >= '0' && ch <= '9') {
      // return the number pressed
      delwin(win);
      return ch - '0';
    }
  }
  return 0;
}

// displays the main menu
int display_menu() {
  // the contents of the dialog
  char menu[][CONTENT_WIDTH] = {
  "%i) Start the game",
  "%i) Controls",
  "%i) Help",
  "%i) Highscores",
  "%i) Exit"
  };
  
  // create a numbered dialog
  return create_numbered_dialog("MENU", (char *)menu, 5);
}

// displays the controls dialog
void display_controls() {
  // the dialog contents
  char controls[][CONTENT_WIDTH] = {
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
  create_enter_dialog("CONTROLS", (const char *)controls, 11);
}

// displays the help menu
void display_help() {
  // the contents of the dialog
  char help[][CONTENT_WIDTH] = {
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

  // create the dialog
  create_enter_dialog("HELP", (const char *)help, 11);
}

// displays the highscore dialog
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


// displays the pause dialog
int pause_dialog() {
  // the contents of the dialog
  char dialog[][CONTENT_WIDTH] = {
  "%i) Resume",
  "%i) Exit"
  };

  // display the numbered dialog
  return create_numbered_dialog("PAUSE", (char *)dialog, 2);
}

void show_highscores() {
  int num, i;
  HIGHSCORE *highscore = read_highscore(&num);
  if(num > 14) {
    num = 14;
  }
  char *highscore_table = calloc((num + 2) * CONTENT_WIDTH, sizeof(char));
  snprintf(highscore_table, CONTENT_WIDTH, "POS            NAME  PTS   SEC  SCORE");
  snprintf(highscore_table + CONTENT_WIDTH, CONTENT_WIDTH, "-------------------------------------");
  for(i = 0; i < num; i++) {
    snprintf(highscore_table + (i + 2) * CONTENT_WIDTH,
      CONTENT_WIDTH, "%2i. %15s %4i  %4li  %5i",
      i + 1,
      highscore[i].name,
      highscore[i].points,
      highscore[i].time_sec,
      highscore[i].highscore);
  }
  create_enter_dialog("HIGHSCORES", highscore_table, num + 2);
}

void enter_string(char *buf, int length) {
  WINDOW *win = create_dialog_window("enter your name:");
  echo();
  mvwgetnstr(win, 3, 1, buf, length);
  noecho();
  delwin(win);
}
