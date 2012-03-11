
#ifndef _TYPES_H
#define _TYPES_H

#include <time.h>
#include <curses.h>
#include <stdlib.h>


// represents a direction on a 2d map
typedef enum {
  DIR_UP = '^',
  DIR_RIGHT = '>',
  DIR_DOWN = 'v',
  DIR_LEFT = '<'
} direction;

// represents a single fruit

struct game;

typedef struct fruit {
  WINDOW *win;
  void (*effect)(struct game*);
} FRUIT;

// represents a collection of fruit, which can be eaten by the snake
typedef struct fruits {
  FRUIT* fruits;
  int length;
} FRUITS;

// the snake
typedef struct snake {
  WINDOW** parts;
  int length;
  direction dir;
  int grow;
} SNAKE;

// a struct containing all the data needed for a game
typedef struct game {
  FRUITS fruits;
  SNAKE snake;
  time_t started;
  time_t ended;
  int highscore;
} GAME;

#include "glog.h"

#endif /* _TYPES_H */
