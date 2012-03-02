#include <time.h>
#include <signal.h>
#include <curses.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef enum {
  DIR_UP = 0,
  DIR_RIGHT = 1,
  DIR_DOWN = 2,
  DIR_LEFT = 3
} direction;

typedef struct fruits {
  WINDOW** fruits;
  int length;
} FRUITS;

typedef struct snake {
  WINDOW** parts;
  int length;
  direction dir;
  int grow;
} SNAKE;

typedef struct game {
  FRUITS fruits;
  SNAKE snake;
  time_t started;
  time_t ended;
  int highscore;
} GAME;

#define MAX_DIR 4

void kill_game(GAME *game);
void kill_snake(SNAKE *snake);
void kill_fruits(FRUITS* fruits);
void kill_fruit(FRUITS *fruits, int posy, int posx);
void grow_snake(SNAKE *snake, int posy, int posx);
int move_snake(GAME *game);
WINDOW *snake_part_is_on(SNAKE *snake, int posy, int posx);
WINDOW *fruit_is_on(FRUITS *fruits, int posy, int posx);
void display_highscore();
void glog(const char *format, ... );

#define LOG_FILE "game.log"
void glog(const char *format, ... ) {
  va_list args;
  static FILE *logfile = NULL;

  char *fformat;
  int length = strlen(format) + 2;
 
  if(logfile == NULL) {
    logfile = fopen(LOG_FILE, "w");
    if(logfile == NULL) {
      fprintf(stderr, "Cannot open logfile %s\n", LOG_FILE);
      exit(EXIT_FAILURE);
    }
  }

  if(format != NULL) {
    fformat = malloc(sizeof(char) * length);
    strncpy(fformat, format, length - 2);
    fformat[length - 2] = '\n';
    fformat[length - 1] = '\0';

    va_start(args, format);
    vfprintf(logfile, fformat, args);
    fflush(logfile);
    free(fformat);
    va_end(args);
  } else {
    fclose(logfile);
  }

}

void kill_game(GAME *game) {
  kill_fruits(&game->fruits);
  kill_snake(&game->snake);
}

void kill_fruits(FRUITS* fruits) {
  int i;

  for(i = 0; i < fruits->length; i++) {
    delwin(fruits->fruits[i]);
  }
  fruits->length = 0;
  free(fruits->fruits);
}

void kill_fruit(FRUITS *fruits, int posy, int posx) {
  WINDOW *fruit;
  int i;
  if((fruit = fruit_is_on(fruits, posy, posx)) != NULL) {
    for(i = 0; i < fruits->length; i++) {
      if(fruits->fruits[i] == fruit) {
        for(i++; i < fruits->length; i++) {
          fruits->fruits[i-1] = fruits->fruits[i];
        }
        fruits->fruits = realloc(fruits->fruits, --fruits->length * sizeof(WINDOW*));
        break;
      }
    }
  }
}
  

WINDOW *fruit_is_on(FRUITS *fruits, int posy, int posx) {
  int i;
  int y,x;

  for(i = 0; i < fruits->length; i++) {
    getbegyx(fruits->fruits[i], y, x);
    if(posy == y && posx == x) {
      return fruits->fruits[i];
    }
  }
  return NULL;
}

void kill_snake(SNAKE *snake) {
  free(snake->parts);
}


WINDOW *snake_part_is_on(SNAKE *snake, int posy, int posx) {
  int i;
  int cury, curx;
  for(i = 0; i < snake->length; i++) {
    getbegyx(snake->parts[i], cury, curx);
    if(cury == posy && curx == posx) {
      return snake->parts[i];
    }
  }
  return NULL;
}

void grow_snake(SNAKE *snake, int posy, int posx) {
  char schr = 'O';
  if(snake->length == 0) {
    snake->parts = malloc(sizeof(WINDOW*) * ++snake->length);
    schr = '0';
  } else {
    snake->parts = realloc(snake->parts, sizeof(WINDOW*) * ++snake->length);
  }
  

  snake->parts[snake->length - 1] = newwin(1, 1, posy, posx);
  wprintw(snake->parts[snake->length - 1], "%c", schr);
  wrefresh(snake->parts[snake->length - 1]);
}

int grow_fruit(GAME* game) {
  int y,x,randy,randx;
  getmaxyx(stdscr, y, x);
  y--;
  x--;
  do {
    randy = (rand() % y) + 1;
    randx = (rand() % x) + 1;
  } while (snake_part_is_on(&game->snake, randy, randx) != NULL && fruit_is_on(&game->fruits, randy, randx) != NULL);

  if(game->fruits.length == 0) {
    game->fruits.fruits = malloc(sizeof(WINDOW*) * ++game->fruits.length);
  } else {
    game->fruits.fruits = realloc(game->fruits.fruits, sizeof(WINDOW*) * ++game->fruits.length);
  }
  
  game->fruits.fruits[game->fruits.length - 1] = newwin(1, 1, randy, randx);
  wprintw(game->fruits.fruits[game->fruits.length - 1], "%c", 'x');
  wrefresh(game->fruits.fruits[game->fruits.length - 1]);

}

#define EVENTS 3

int check_fruit_collision(GAME* game, int cury, int curx) {
  return fruit_is_on(&game->fruits, cury, curx) != NULL;
}
 
int check_fruit_collision_handler(GAME* game, int cury, int curx) {
  kill_fruit(&game->fruits, cury, curx);
  grow_fruit(game);
  game->snake.grow++;
  game->highscore++;
  glog("fruit collision");
  return 1;
}

int check_border_collision(GAME* game, int cury, int curx) {
  int y,x,back;
  getmaxyx(stdscr, y, x);
  y--;
  x--;
  return cury <= 0 || curx <= 0 || cury >= y || curx >= x;
}

int check_border_collision_handler(GAME* game, int cury, int curx) {
  glog("border collision %i %i", cury, curx);
  return 0;
}

int check_self_collision(GAME* game, int cury, int curx) {
  int back = 0;
  WINDOW* on;
  glog("SNAKE PART ON %i %i", cury, curx);
  back = ! ((on = snake_part_is_on(&game->snake, cury, curx)) == NULL || on == game->snake.parts[game->snake.length - 1]);
  glog("returning %i", back);
  return back;
}

int check_self_collision_handler(GAME* game, int cury, int curx) {
  glog("self collision");
  return 0;
}

int move_snake(GAME *game) {
  int (*collision_checks[EVENTS]) (GAME*, int, int) = {
    check_self_collision,
    check_border_collision,
    check_fruit_collision
  };

  int (*collision_handlers[EVENTS])(GAME*, int, int) = {
    check_self_collision_handler,
    check_border_collision_handler,
    check_fruit_collision_handler
  };
  int success = 1;
  int i;
  int test = 0;
  int xdiff = game->snake.dir == DIR_LEFT ? -1 : (game->snake.dir == DIR_RIGHT ? 1 : 0);
  int ydiff = game->snake.dir == DIR_UP ? -1 : (game->snake.dir == DIR_DOWN ? 1 : 0);
  glog("diff y: %i x: %i", ydiff, xdiff);
  int curx;
  int cury;
  int tmpy;
  int tmpx;
  getbegyx(game->snake.parts[0], cury, curx);
  cury += ydiff;
  curx += xdiff;
  for(i = 0; i < EVENTS && success; i++) {
    if(test = collision_checks[i](game, cury, curx)) {
      glog("dafuq? %i", test);
      if(!collision_handlers[i](game, cury, curx)) {
        success = 0;
      }
    }
  }
  if(success) {
    glog("y: %i, x: %i", cury, curx);
    mvwin(game->snake.parts[0], cury, curx);
    glog("moved first window");
    wsyncup(game->snake.parts[0]);
    wrefresh(game->snake.parts[0]);
    glog("refreshed first window");
    for(i = 1; i < game->snake.length; i++) {
      getbegyx(game->snake.parts[i], tmpy, tmpx);
      mvwin(game->snake.parts[i], cury, curx);
      glog("moved window");
      glog("x: %i, y: %i", cury, curx);
      cury = tmpy;
      curx = tmpx;
      wsyncup(game->snake.parts[i]);
      wrefresh(game->snake.parts[i]);
      glog("refreshed window");
    }


    glog("going to grow?");
    if(game->snake.grow > 0) {
      glog("yes");
      grow_snake(&game->snake, cury, curx);
      game->snake.grow--;
    } else {
      glog("no");
      WINDOW *cur = newwin(1, 1, cury, curx);
      wprintw(cur, "%c", ' ');
      wrefresh(cur);
      delwin(cur);
    }
  }
  glog("back");
  return success;
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

void run() {
  int ch, ich;
  int rows;
  int columns;
  int success = 1;
  long nsec_before = 1000000;
  struct itimerspec delay;
  struct itimerspec get_delay;
  delay.it_interval.tv_sec = 0;
  delay.it_interval.tv_nsec = 10000000;
  delay.it_value.tv_sec = delay.it_interval.tv_sec;
  delay.it_value.tv_nsec = delay.it_interval.tv_nsec;
  timer_t timer;
  struct sigevent event = {};
  event.sigev_notify = SIGEV_NONE;

  GAME game = {};
  game.highscore = 0;

  int x,y;
  border('#', '#', '#', '#', '#', '#', '#', '#');
  refresh();

  getmaxyx(stdscr, rows, columns); // get the dimensions of the terminal

  // place the snake in the middle of the game field
  grow_snake(&game.snake, rows / 2, columns / 2);
  grow_snake(&game.snake, rows / 2 + 1, columns / 2);
  glog("size %i", game.snake.length);
  game.snake.dir = DIR_LEFT;

  int i; 
  for(i = 0; i < 50; i++) {
    grow_fruit(&game);
  }
  
  time(&game.started);

  int timerc = timer_create(CLOCK_REALTIME, &event, &timer);
  glog("timerc: %i", timerc);
  timer_settime(timer, 0, &delay, NULL);
  
  while((ich = getch()) && success && ch != 'x') {
    timer_gettime(timer, &get_delay);
    if(ich != ERR) {
      ch = ich;
    }
    if((int)get_delay.it_value.tv_sec == 0 && (int)get_delay.it_value.tv_nsec > nsec_before) {
      if( ch == KEY_UP && game.snake.dir != DIR_DOWN) {
        game.snake.dir = DIR_UP;
      } else if( ch == KEY_LEFT && game.snake.dir != DIR_RIGHT) {
        game.snake.dir = DIR_LEFT;
      } else if( ch == KEY_RIGHT && game.snake.dir != DIR_LEFT) {
        game.snake.dir = DIR_RIGHT;
      } else if( ch ==  KEY_DOWN && game.snake.dir != DIR_UP) {
        game.snake.dir = DIR_DOWN;
      }
      success = move_snake(&game);
      refresh();
      timer_settime(timer, 0, &delay, NULL);
    }
    nsec_before = (long)get_delay.it_value.tv_nsec;
  }
  timer_delete(timer);
  time(&game.ended);
  display_highscore(&game);
  kill_game(&game);
}

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

  glog("void");
  glog("void%s", "ness");
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
