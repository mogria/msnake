#include "game.h"
#include "events.h"
#include "fruits.h"
#include "snake.h"
#include "main.h"
#include "dialog.h"
#include "highscore.h"
#include "current_utc_time.h"

// end the game, free the resources
void kill_game(GAME *game) {
  kill_fruits(&game->fruits);
  kill_snake(&game->snake);
}


// calculate the difference between two timespec structs
long long timeval_diff(struct timespec* tv1, struct timespec* tv2) {
  long long diff = 0;
  // calculate the diffrence of the nanoseconds
  long nano = (tv2->tv_nsec - tv1->tv_nsec);
  int add = 0;

  // some calculations if the nanoseconds of struct 1 were bigger
  if(nano < 0) {
    nano = 1000000000 + nano;
    add = 1;
  }
  diff = nano;
  // add the seconds
  diff += (tv2->tv_sec - tv1->tv_sec - add) * 1000000000;
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
  char playername[HIGHSCORE_NAME_LENGTH] = {};

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
  // get the current time
  current_utc_time(&last_time);
  while((ich = getch()) && success && ch != 'x') {
    
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
    current_utc_time(&current_time);
    res = timeval_diff(&last_time, &current_time);
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

    // 'p' pressed
    if(ich == 'p') {
      // show the pause dialog
      switch(pause_dialog()) {
        case 2:
          // leave the game if '2' is pressed
          success = 0;
        default:
          // redraw the screen on resume
          redraw_game(&game);
          break;
      }
    }
  }

  // get the time when the game has ended
  time(&game.ended);

  // display the highscore dialog
  display_highscore(&game);

  // let the player input his name
  enter_string(playername, HIGHSCORE_NAME_LENGTH);

  // has a name been entered? if not don't create a menu entry
  if(playername[0]) {
    add_highscore(playername, game.highscore, game.ended - game.started);
  }
  // free all the resources reserved in the game struct
  kill_game(&game);
}

// redraw the whole screen
void redraw_game(GAME *game) {
  // redraw the main window (containg the border and stuff)
  redrawwin(stdscr);
  refresh();
  
  // redraw the fruits
  redraw_fruits(&game->fruits);

  // redraw the snake
  redraw_snake(&game->snake);
}
