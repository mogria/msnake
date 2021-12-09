#include "fruits.h"
#include "effects.h"
#include "events.h"
#include "snake.h"

#include <stdlib.h>

// make sure to provide enough space to store
// all fruits when all fields contain a fruit
void manage_fruit_memory(GAME* game) {
  size_t max_num_fruits_to_allocate = game->rows * game->columns;
  if(game->fruits.allocated == 0) {
    // initialize the array with malloc if it is the first fruit in the array
    game->fruits.fruits = malloc(sizeof(FRUIT) * max_num_fruits_to_allocate);
    bzero(game->fruits.fruits, sizeof(FRUIT) * max_num_fruits_to_allocate);
  } else if(game->fruits.allocated != max_num_fruits_to_allocate) {
    // allocate more memory
    game->fruits.fruits = realloc(game->fruits.fruits, max_num_fruits_to_allocate);
    size_t new_fruits = max_num_fruits_to_allocate - game->fruits.allocated;
    bzero(&game->fruits.fruits[game->fruits.allocated],
            sizeof(FRUIT) * new_fruits);
  }
  game->fruits.allocated = max_num_fruits_to_allocate;
}

FRUIT *get_fruit_addr(GAME* game, int y, int x) {
  return &game->fruits.fruits[y * game->rows + x];
}

static inline void
draw_fruit(FRUIT *fruit)
{
  POINT *point = &fruit->point;
  //clear space if not a fruit anymore
  if(point->ch == '\0') {
    mvprintw(point->y, point->x, " ");
    return;
  }

  // set the color and brightness
  attron(A_BOLD | COLOR_PAIR(fruit->color));
  // print the char on the main window
  mvprintw(point->y, point->x, "%c", point->ch);
}

// free the resources needed by the fruits
void kill_fruits(FRUITS* fruits) {
  // free the allocated memory
  free(fruits->fruits);
}

// remove a single fruit from the game by it's position
void kill_fruit(FRUIT *fruit) {
    bzero(fruit, sizeof(FRUIT));
    draw_fruit(fruit);
}

// returns a FRUIT if one is found on the given point, else NULL
FRUIT *get_fruit_on(GAME *game, int posy, int posx) {
  FRUIT* fruit = get_fruit_addr(game, posy, posx);
  if(fruit->point.ch == 0) {
      return NULL;
  }
  return fruit;
}

bool can_fruit_grow(GAME *game, int y, int x) {
    if(snake_part_is_on(&game->snake, y, x) != NULL) {
        return false;
    }
    if(get_fruit_on(game, y, x) != NULL) {
        return false;
    }
    if(check_extended_border_collision(game, y, x)) {
        return false;
    }

    return true;
}

// create a new fruit in the game
void grow_fruit(GAME* game) {
  if(game->fruits.allocated == 0) {
    manage_fruit_memory(game);
  }

  int randy,randx;
  // generate a new random position until a empty spot is found
  do {
    randy = rand() % game->rows;
    randx = rand() % game->columns;

    // is nothing else there in the generated position?
  } while(!can_fruit_grow(game, randy, randx));

  manage_fruit_memory(game);

  // get a filled struct (containing the displayed char, the effect & co.) of the new fruit
  FRUIT *fruit = get_fruit_addr(game, randy, randx);
  initialize_fruit(fruit, randy, randx);
  draw_fruit(fruit);
}


// fill in data into a fruit struct
void initialize_fruit(FRUIT *fruit, int posy, int posx) {
  // how the diffrent fruits are displayed
  static char chars[EFFECTS] = {'x', '@', '%', '&'};
  static int colors[EFFECTS] = { 4 ,  6 ,  3 ,  5 }; // see color definitions in the end of main.c
  // the different effects of the fruits
  static void (*effects[EFFECTS])(GAME *) = {
    normal_effect, // see effects.c
    double_grow,
    mega_food,
    eat_boost
  };
  // the chance a certain fruit appears
  static int chance[EFFECTS] = {100, 25, 5, 2};
  // the sum of all the chances
  static int max_chance = 132;

  int i = 0;
  int sum = 0;

  // generate a random number
  int random = rand() % max_chance;
  // get the number of the fruit out of the random number
  // the index will increase every time the random value is greater
  // than the sum of all elements before the index and the element with the index
  // there is also a bounds check for the array
  while(random >= (sum += chance[i]) && i + 1 < EFFECTS) {
    i++; // next index
  };

  // create a new point
  fruit->point.x = posx;
  fruit->point.y = posy;
  fruit->point.ch = chars[i];
  // save a color
  fruit->color = colors[i];
  // set an effect
  fruit->effect = effects[i];

  // draw the fruit
  draw_fruit(fruit);
}

// redraw all the fruits on the screen
void redraw_fruits(GAME *game) {
  // iterate through each fruit.
  for(int i = 0; i < game->fruits.allocated; i++) {
    // redraw it!
    draw_fruit(&game->fruits.fruits[i]);
  }
}
