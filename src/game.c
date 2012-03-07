#include "game.h"
#include "fruits.h"
#include "snake.h"
#include <time.h>

void kill_game(GAME *game) {
  kill_fruits(&game->fruits);
  kill_snake(&game->snake);
}


long long timeval_diff(struct timeval* tv1, struct timeval* tv2) {
  long long diff = 0;
  long long test;
  suseconds_t mikro = (tv2->tv_usec - tv1->tv_usec);
  int add = 0;
  if(mikro < 0) {
    mikro = 1000000 + mikro;
    add = 1;
  }
  diff |= mikro;
  diff |= (tv2->tv_sec - tv1->tv_sec - add) << (sizeof(suseconds_t) * 8) ;
  return diff;
}

void run() {
  int ch, ich, rows, columns, success = 1;

  // some variables for the timer (inclusive the interval)
  struct timeval last_time = {};
  struct timeval current_time = {};
  long long interval = 10000;

  // create the game struct
  GAME game = {};

  // create a border
  border('#', '#', '#', '#', '#', '#', '#', '#');
  refresh();

  // get the dimensions of the terminal
  getmaxyx(stdscr, rows, columns);

  // place the snake in the middle of the game field
  grow_snake(&game.snake, rows / 2, columns / 2);
  grow_snake(&game.snake, rows / 2 + 1, columns / 2);
  glog("size %i", game.snake.length);
  game.snake.dir = DIR_LEFT;

  // create some fruits on the screen
  // NOM, NOM, NOM
  int i; 
  for(i = 0; i < 50; i++) {
    grow_fruit(&game);
  }
  
  // get the time when the game started
  time(&game.started);

  gettimeofday(&last_time);
  while((ich = getch()) && success && ch != 'x') {
    // key typed?
    if(ich != ERR) {
      ch = ich;
    }
    // check if we have an overrun
    gettimeofday(&current_time);
    long long res = timeval_diff(&last_time, &current_time);
    glog("%lli =  %i, %i.%i %i.%i", res, last_time.tv_sec, last_time.tv_usec, current_time.tv_sec, current_time.tv_usec);
    if( res > interval) {
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

  // get the time when the game has ended
  time(&game.ended);

  // display the highscore dialog
  display_highscore(&game);
  // free all the resources reserved in the game struct
  kill_game(&game);
}
