#include <time.h>
#include <signal.h>
#include <curses.h>
#include <stdlib.h>

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

FILE *logf;

void kill_game(GAME *game);
void kill_snake(SNAKE *snake);
void kill_fruits(FRUITS* fruits);
void kill_fruit(FRUITS *fruits, int posy, int posx);
void grow_snake(SNAKE *snake, int posy, int posx);
int move_snake(GAME *game);
WINDOW *snake_part_is_on(SNAKE *snake, int posy, int posx);
WINDOW *fruit_is_on(FRUITS *fruits, int posy, int posx);

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
  wprintw(game->fruits.fruits[game->fruits.length - 1], "%c", '@');
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
  fprintf(logf, "fruit collision\n");fflush(logf);
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
  fprintf(logf, "border collision %i %i\n", cury, curx);fflush(logf);
  return 0;
}

int check_self_collision(GAME* game, int cury, int curx) {
  int back = 0;
  WINDOW* on;
  fprintf(logf, "SNAKE PART ON %i %i\n", cury, curx);fflush(logf);
  back = ! ((on = snake_part_is_on(&game->snake, cury, curx)) == NULL || on == game->snake.parts[game->snake.length - 1]);
  fprintf(logf, "returning %i\n", back);
  return back;
}

int check_self_collision_handler(GAME* game, int cury, int curx) {
  fprintf(logf, "self collision\n");fflush(logf);
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
  fprintf(logf, "diff y: %i x: %i\n", ydiff, xdiff);fflush(logf);
  int curx;
  int cury;
  int tmpy;
  int tmpx;
  getbegyx(game->snake.parts[0], cury, curx);
  cury += ydiff;
  curx += xdiff;
  for(i = 0; i < EVENTS && success; i++) {
    if(test = collision_checks[i](game, cury, curx)) {
      fprintf(logf, "dafuq? %i\n", test);fflush(logf);
      if(!collision_handlers[i](game, cury, curx)) {
        success = 0;
      }
    }
  }
  if(success) {
    fprintf(logf, "y: %i, x: %i\n", cury, curx);fflush(logf);
    mvwin(game->snake.parts[0], cury, curx);
    fprintf(logf, "moved first window\n");fflush(logf);
    wsyncup(game->snake.parts[0]);
    wrefresh(game->snake.parts[0]);
    fprintf(logf, "refreshed first window\n");fflush(logf);
    for(i = 1; i < game->snake.length; i++) {
      getbegyx(game->snake.parts[i], tmpy, tmpx);
      mvwin(game->snake.parts[i], cury, curx);
      fprintf(logf, "moved window\n");fflush(logf);
      fprintf(logf, "x: %i, y: %i\n", cury, curx);fflush(logf);
      cury = tmpy;
      curx = tmpx;
      wsyncup(game->snake.parts[i]);
      wrefresh(game->snake.parts[i]);
      fprintf(logf, "refreshed window\n");fflush(logf);
    }


    fprintf(logf, "going to grow?\n");fflush(logf);
    if(game->snake.grow > 0) {
      fprintf(logf, "yes\n");fflush(logf);
      grow_snake(&game->snake, cury, curx);
      game->snake.grow--;
    } else {
      fprintf(logf, "no\n");fflush(logf);
      WINDOW *cur = newwin(1, 1, cury, curx);
      wprintw(cur, "%c", ' ');
      wrefresh(cur);
      delwin(cur);
    }
  }
  fprintf(logf, "back\n");fflush(logf);
  return success;
}

int main () {
  int ch, ich;
  int rows;
  int columns;
  int success = 1;
  long nsec_before = 1000000;
  struct itimerspec delay;
  struct itimerspec get_delay;
  delay.it_interval.tv_sec = 0;
  delay.it_interval.tv_nsec = 50000000;
  delay.it_value.tv_sec = delay.it_interval.tv_sec;
  delay.it_value.tv_nsec = delay.it_interval.tv_nsec;
  timer_t timer;
  struct sigevent event = {};
  event.sigev_notify = SIGEV_NONE;

  GAME game = {};
  game.highscore = 0;
  time(&game.started);

  srand(time(NULL));

  logf = fopen("game.log", "w");
  initscr();
  nodelay(stdscr, TRUE);
  noecho();
  keypad(stdscr, TRUE);

  getmaxyx(stdscr, rows, columns);

  int x,y;

  border('#', '#', '#', '#', '#', '#', '#', '#');
  refresh();

  grow_snake(&game.snake, rows / 2, columns / 2);
  grow_snake(&game.snake, rows / 2 + 1, columns / 2);
  fprintf(logf, "size %i\n", game.snake.length);fflush(logf);
  //snake.dir = DIR_LEFT;

  grow_fruit(&game);
  grow_fruit(&game);
  grow_fruit(&game);
  grow_fruit(&game);
  grow_fruit(&game);
  grow_fruit(&game);
  grow_fruit(&game);
  grow_fruit(&game);
  grow_fruit(&game);
  grow_fruit(&game);
  grow_fruit(&game);
  grow_fruit(&game);
  grow_fruit(&game);

  int timerc = timer_create(CLOCK_REALTIME, &event, &timer);
  fprintf(logf, "timerc: %i\n", timerc);
  timer_settime(timer, 0, &delay, NULL);
  
  while((ich = getch()) && success && ch != 'x') {
    timer_gettime(timer, &get_delay);
    //fprintf(logf, "interval: %i.%i valute: %i.%i\n", (int)get_delay.it_interval.tv_sec, (int)get_delay.it_interval.tv_nsec, (int)get_delay.it_value.tv_sec, (int)get_delay.it_value.tv_nsec);
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
  
  //display_highscore(&game);

  getch();
  endwin();
  kill_game(&game);
  fclose(logf);
  return 0;
}
