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

// wait until enter is pressed
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
  "%i) Highscores",
  "%i) Controls",
  "%i) Help",
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

// display a table containing the highscores of the players
void show_highscores() {
  int num, i;
  // read all the submitted highscores from a file
  HIGHSCORE *highscore = read_highscore(&num);

  // limit the highscore display to (at the moment 14) entries
  if(num > DIALOG_HEIGHT - 6) {
    num = DIALOG_HEIGHT - 6;
  }
  // create a char array for the content of the highscore table
  char *highscore_table = calloc((num + 2) * CONTENT_WIDTH, sizeof(char));
  // insert the table heading
  snprintf(highscore_table, CONTENT_WIDTH, "POS            NAME  PTS   SEC  SCORE");
  snprintf(highscore_table + CONTENT_WIDTH, CONTENT_WIDTH, "-------------------------------------");
  // insert the highscores
  for(i = 0; i < num; i++) {
    // insert a highscore record
    snprintf(highscore_table + (i + 2) * CONTENT_WIDTH,
      CONTENT_WIDTH, "%2i. %15s %4i  %4li  %5i",
      i + 1,
      highscore[i].name,
      highscore[i].points,
      highscore[i].time_sec,
      highscore[i].highscore);
  }
  // create a dialog and display the data
  create_enter_dialog("HIGHSCORES", highscore_table, num + 2);
  // free the resources
  free(highscore_table);
}

void enter_string(char *title, char *content, int lines, int posy, int posx, char *buf, int length) {
  WINDOW *win = create_dialog_window(title);
  int i;

  //print out the content
  for(i = 0; i < lines; i++) {
    mvwprintw(win, i + 3, 1, "%s", &content[i * CONTENT_WIDTH]);
  }

  // turn on the echo on the screen
  echo();
  // get the name
  mvwgetnstr(win, posy, posx, buf, length);
  // turn the echo off again
  noecho();
  // delete the dialog
  delwin(win);
}

// displays the highscore dialog
void display_highscore(GAME *game, char *buf, int length) {
  int lines = 6;
  // allocate enougth memory
  char *content = calloc(lines * CONTENT_WIDTH, sizeof(char));

  // generate the window contents
  snprintf(content                    , CONTENT_WIDTH, "snake length : %i", game->snake.length);
  snprintf(content + CONTENT_WIDTH    , CONTENT_WIDTH, "points       : %i", game->highscore);
  snprintf(content + CONTENT_WIDTH * 2, CONTENT_WIDTH, "time         : %lis", game->ended - game->started - game->paused);
  snprintf(content + CONTENT_WIDTH * 3, CONTENT_WIDTH, "highscore    : %i", calculate_score(game->highscore, game->ended - game->started - game->paused));
  snprintf(content + CONTENT_WIDTH * 4, CONTENT_WIDTH, "----------------------------------------");
  snprintf(content + CONTENT_WIDTH * 5, CONTENT_WIDTH, "enter your name to be added to the highscore");
  snprintf(content + CONTENT_WIDTH * 5, CONTENT_WIDTH, "Name: ");

  // show the dialog
  enter_string("GAME OVER", content, lines, 8, 7, buf, length);

  // free the memory
  free(content);
}
