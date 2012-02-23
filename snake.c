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
} SNAKE;


#define MAX_DIR 4

FILE *logf;

void turn_left(direction *d);
void turn_right(direction *d);
void kill_snake(SNAKE *snake);
void grow_snake(SNAKE *snake, int posy, int posx);
int move_snake(SNAKE *snake, int grow);
WINDOW *snake_part_is_on(SNAKE *snake, int posy, int posx);
void kill_fruits(FRUITS* fruits);
void kill_fruit(FRUITS *fruits, int posy, int posx);
WINDOW *fruit_is_on(FRUITS *fruits, int posy, int posx);

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

void turn_left(direction *d) {
  *d  = (*d + 1) % MAX_DIR;

}

void turn_right(direction *d) {
  *d  = (*d - 1) % MAX_DIR;
}

void kill_snake(SNAKE *snake) {
  free(snake->parts);
}

void grow_snake(SNAKE *snake, int posy, int posx) {
  if(snake->length == 0) {
    snake->parts = malloc(sizeof(WINDOW*) * ++snake->length);
  } else {
    snake->parts = realloc(snake->parts, sizeof(WINDOW*) * ++snake->length);
  }
  

  snake->parts[snake->length - 1] = newwin(1, 1, posy, posx);
  wprintw(snake->parts[snake->length - 1], "%c", 'O');
  wrefresh(snake->parts[snake->length - 1]);
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

int grow_fruit(SNAKE* snake, FRUITS *fruits) {
  int y,x,randy,randx;
  getmaxyx(stdscr, y, x);
  y--;
  x--;
  fprintf(logf, "beg x: %i, y: %i\n", x, y);
  do {
    randy = (rand() % y) + 1;
    randx = (rand() % x) + 1;
  } while (snake_part_is_on(snake, randy, randx) != NULL && fruit_is_on(fruits, randy, randx) != NULL);
  fprintf(logf, "rand: %i\n", rand());
  fprintf(logf, "x: %i, y: %i\n", randx, randy);

  if(fruits->length == 0) {
    fruits->fruits = malloc(sizeof(WINDOW*) * ++fruits->length);
  } else {
    fruits->fruits = realloc(fruits->fruits, sizeof(WINDOW*) * ++fruits->length);
  }
  
  fruits->fruits[fruits->length - 1] = newwin(1, 1, randy, randx);
  wprintw(fruits->fruits[fruits->length - 1], "%c", '.');
  wrefresh(fruits->fruits[fruits->length - 1]);

}

#define EVENTS 2
int check_border_collision(SNAKE *snake, int cury, int curx) {
  int y,x;
  getmaxyx(stdscr, y, x);
  y--;
  x--;
  return cury <= 0 || curx <= 0 || cury >= y || curx >= x;
}

int check_border_collision_handler(SNAKE *snake) {
  return 0;
}

int check_self_collision(SNAKE *snake, int cury, int curx) {
  WINDOW* on;
  return ! ((on = snake_part_is_on(snake, cury, curx)) == NULL || on == snake->parts[snake->length - 1]);
}

int check_self_collision_handler(SNAKE *snake) {
  return 0;
}

int move_snake(SNAKE *snake, int grow) {
  int (*collision_checks[EVENTS])(SNAKE*,int,int) = {
    check_border_collision,
    check_self_collision
  };

  int (*collision_handlers[EVENTS])(SNAKE*) = {
    check_self_collision_handler,
    check_border_collision_handler
  };
  int success = 1;
  int i;
  int xdiff = snake->dir == DIR_LEFT ? -1 : (snake->dir == DIR_RIGHT ? 1 : 0);
  int ydiff = snake->dir == DIR_UP ? -1 : (snake->dir == DIR_DOWN ? 1 : 0);
  int curx;
  int cury; 
  int tmpy; 
  int tmpx;
  getbegyx(snake->parts[snake->length -1], cury, curx);
  for(i = 0; i < EVENTS && success; i++) {
    if(collision_checks[i](snake, cury + ydiff, curx + xdiff )) {
      if(!collision_handlers[i](snake)) {
        success = 0;
      }
    }
  }
  if(success) {
    mvwin(snake->parts[snake->length -1], cury + ydiff, curx + xdiff);
    wrefresh(snake->parts[snake->length -1]);
    for(i = snake->length - 2; i >= 0; i--) {
      getbegyx(snake->parts[i], tmpy, tmpx);
      mvwin(snake->parts[i], cury, curx);
      cury = tmpy;
      curx = tmpx;
      wsyncup(snake->parts[i]);
      wrefresh(snake->parts[i]);
    }

    if(grow) {
      grow_snake(snake, cury, curx);
    } else {
      WINDOW *cur = newwin(1, 1, cury, curx);
      wprintw(cur, "%c", ' ');
      wrefresh(cur);
      delwin(cur);
    }
  }
  return success;
}

int main () {
  int ch;
  int rows;
  int columns;
  int success = 1;
  SNAKE snake = {};
  FRUITS fruits = {};

  srand(time(NULL));

  logf = fopen("game.log", "w");
  initscr();
  halfdelay(1);
  noecho();
  //nonl();
  //intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);

  getmaxyx(stdscr, rows, columns);

  int x,y;

  border('#', '#', '#', '#', '#', '#', '#', '#');
  refresh();

  grow_snake(&snake, rows / 2, columns / 2);
  grow_snake(&snake, rows / 2 - 1, columns / 2);
  grow_snake(&snake, rows / 2 - 1, columns / 2 - 1);
  grow_snake(&snake, rows / 2 - 1, columns / 2 - 2);
  grow_snake(&snake, rows / 2 - 1, columns / 2 - 3);
  grow_snake(&snake, rows / 2 - 1, columns / 2 - 4);
  grow_snake(&snake, rows / 2 - 1, columns / 2 - 5);
  grow_snake(&snake, rows / 2 - 1, columns / 2 - 6);
  grow_snake(&snake, rows / 2 - 1, columns / 2 - 7);
  grow_snake(&snake, rows / 2 - 1, columns / 2 - 8);
  grow_snake(&snake, rows / 2 - 1, columns / 2 - 9);
  grow_snake(&snake, rows / 2 - 2, columns / 2 - 9);
  grow_snake(&snake, rows / 2 - 3, columns / 2 - 9);
  grow_snake(&snake, rows / 2 - 4, columns / 2 - 9);
  grow_snake(&snake, rows / 2 - 5, columns / 2 - 9);
  grow_snake(&snake, rows / 2 - 6, columns / 2 - 9);

  grow_fruit(&snake, &fruits);
  grow_fruit(&snake, &fruits);
  grow_fruit(&snake, &fruits);
  grow_fruit(&snake, &fruits);
  grow_fruit(&snake, &fruits);
  grow_fruit(&snake, &fruits);
  grow_fruit(&snake, &fruits);
  grow_fruit(&snake, &fruits);
  grow_fruit(&snake, &fruits);
  grow_fruit(&snake, &fruits);
  grow_fruit(&snake, &fruits);
  
  while((ch = getch()) && success) {
    if( ch == KEY_UP && snake.dir != DIR_DOWN) {
      snake.dir = DIR_UP;
    } else if( ch == KEY_LEFT && snake.dir != DIR_RIGHT) {
      snake.dir = DIR_LEFT;
    } else if( ch == KEY_RIGHT && snake.dir != DIR_LEFT) {
      snake.dir = DIR_RIGHT;
    } else if( ch ==  KEY_DOWN && snake.dir != DIR_UP) {
      snake.dir = DIR_DOWN;
    }
    success = move_snake(&snake, 0);
    refresh();
  }
  getch();
  endwin();
  kill_snake(&snake);
  kill_fruits(&fruits);
  fclose(logf);
  return 0;
}
