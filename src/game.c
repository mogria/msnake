#include "game.h"
#include "fruits.h"
#include "snake.h"
#include <time.h>

void kill_game(GAME *game) {
  kill_fruits(&game->fruits);
  kill_snake(&game->snake);
}

void run() {
  int ch, ich, rows, columns, success = 1;

  // some variables for the timer (inclusive the interval)
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

  // create a timer
  int timerc = timer_create(CLOCK_REALTIME, &event, &timer);
  glog("timerc: %i", timerc);
  timer_settime(timer, 0, &delay, NULL);

  while((ich = getch()) && success && ch != 'x') {
    // key typed?
    if(ich != ERR) {
      ch = ich;
    }
    // check if we have an overrun
    timer_gettime(timer, &get_delay);
    if((int)get_delay.it_value.tv_sec == 0 && (int)get_delay.it_value.tv_nsec > nsec_before) {
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
    }
    nsec_before = (long)get_delay.it_value.tv_nsec;
  }
  // delete the timer
  timer_delete(timer);

  // get the time when the game has ended
  time(&game.ended);

  // display the highscore dialog
  display_highscore(&game);
  // free all the resources reserved in the game struct
  kill_game(&game);
}
