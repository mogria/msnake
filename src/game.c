#include "game.h"
#include "fruits.h"
#include "snake.h"
#include <time.h>

void kill_game(GAME *game) {
  kill_fruits(&game->fruits);
  kill_snake(&game->snake);
}


long long timeval_diff(struct timespec* tv1, struct timespec* tv2) {
  long long diff = 0;
  long long test;
  long nano = (tv2->tv_nsec - tv1->tv_nsec);
  int add = 0;
  if(nano < 0) {
    nano = 1000000000 + nano;
    add = 1;
  }
  diff |= nano;
  diff |= (tv2->tv_sec - tv1->tv_sec - add) * 2 * 32;
  return diff;
}

void run() {
  int ch, ich, y, x, rows, columns, success = 1;

  // some variables for the timer (inclusive the interval)
  struct timespec last_time = {};
  struct timespec current_time = {};
  long long default_interval = 20000000;
  long long interval = default_interval;
  long long res;
  short paused = 0;

  // create the game struct
  GAME game = {};

  // get the dimensions of the terminal
  getmaxyx(stdscr, rows, columns);

  clear();
  // create a border
  for(x = 0; x < columns; x++) {
    for(y = 0; y < rows; y++) {
      if(check_extended_border_collision(&game, y, x)) {
        mvprintw(y, x, "#");
      }
    }
  }
  refresh();

  // place the snake in the middle of the game field
  grow_snake(&game.snake, rows / 2, columns / 2);
  game.snake.dir = DIR_LEFT;

  // create some fruits on the screen
  // NOM, NOM, NOM
  int i; 
  for(i = 0; i < 50; i++) {
    grow_fruit(&game);
  }
  
  // get the time when the game started
  time(&game.started);
  clock_gettime(CLOCK_REALTIME, &last_time);
  while((ich = getch()) && success && ch != 'x') {
    if(ich == 'p') {
      paused = !paused;
    }
    if(!paused) {
      // key typed?
      if(ich == ERR) {
      } else if(ich == '0') {
          interval = default_interval;
      } else if(ich == '8') {
          interval *= 1.1;
      } else if(ich == '9') {
          interval *= 0.9;
      } else {
        ch = ich;
      }
      // check if we have an overrun
      clock_gettime(CLOCK_REALTIME, &current_time);
      res = timeval_diff(&last_time, &current_time);
      glog("%lli", res);
      if(res > interval) {
        // new direction? 
        if((ch == KEY_UP || ch == 'w') && game.snake.dir != DIR_DOWN) {
          game.snake.dir = DIR_UP;
        } else if((ch == KEY_LEFT || ch == 'a') && game.snake.dir != DIR_RIGHT) {
          game.snake.dir = DIR_LEFT;
        } else if((ch == KEY_RIGHT || ch == 'd') && game.snake.dir != DIR_LEFT) {
          game.snake.dir = DIR_RIGHT;
        } else if((ch == KEY_DOWN || ch == 's') && game.snake.dir != DIR_UP) {
          game.snake.dir = DIR_DOWN;
        }
        // move the snake
        success = move_snake(&game);

        // refresh the screen
        refresh();
        last_time = current_time;
      }
    }
  }

  // get the time when the game has ended
  time(&game.ended);

  // display the highscore dialog
  display_highscore(&game);
  // free all the resources reserved in the game struct
  kill_game(&game);
}
