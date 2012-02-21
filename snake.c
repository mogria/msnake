#include <curses.h>
#include <stdlib.h>

typedef enum {
  DIR_UP = 0,
  DIR_RIGHT = 1,
  DIR_DOWN = 2,
  DIR_LEFT = 3
} direction;

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
void move_snake(SNAKE *snake, int grow);

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

void move_snake(SNAKE *snake, int grow) {
  int i;
  int xdiff = snake->dir == DIR_LEFT ? -1 : (snake->dir == DIR_RIGHT ? 1 : 0);
  int ydiff = snake->dir == DIR_UP ? -1 : (snake->dir == DIR_DOWN ? 1 : 0);
  fprintf(logf, "diff (%i|%i)\n", xdiff, ydiff);
  int curx;
  int cury; 
  int tmpy; 
  int tmpx;
  getbegyx(snake->parts[snake->length -1], cury, curx);
  fprintf(logf, "current_pos (%i%i)\n", curx, cury);
  mvwin(snake->parts[snake->length -1], cury + ydiff, curx + xdiff);
  fprintf(logf, "setting to (%i|%i)\n", curx + xdiff, cury + ydiff);
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
    wprintw(cur, "%c", '.');
    wrefresh(cur);
    delwin(cur);
  }
}

int main () {
  int ch;
  int rows;
  int columns;
  SNAKE snake = {};

  logf = fopen("game.log", "w");
  initscr();
  halfdelay(1);
  noecho();
  //nonl();
  //intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);

  getmaxyx(stdscr, rows, columns);

  int x,y;

  // print borders
  for(x = 0; x < columns; x++) {
    for(y = 0; y < rows; y++) {
      char c = '.';
      if(y == 0 || x == 0 || x + 1 == columns || y + 1 == rows) {
        c = '#';
      }
      mvprintw(y, x, "%c", c);
    }
  }
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
  
  while(ch = getch()) {
    fprintf(logf, "got char %c\n", ch);
    if( ch == KEY_UP && snake.dir != DIR_DOWN) {
      snake.dir = DIR_UP;
    } else if( ch == KEY_LEFT && snake.dir != DIR_RIGHT) {
      snake.dir = DIR_LEFT;
    } else if( ch == KEY_RIGHT && snake.dir != DIR_LEFT) {
      snake.dir = DIR_RIGHT;
    } else if( ch ==  KEY_DOWN && snake.dir != DIR_UP) {
      snake.dir = DIR_DOWN;
    }
    fprintf(logf, "new direction %i\n", snake.dir);
    move_snake(&snake, 0);
    refresh();
  }
  getch();
  endwin();
  fclose(logf);
  return 0;
}
