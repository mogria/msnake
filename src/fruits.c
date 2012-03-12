#include "fruits.h"
#include "effects.h"
#include "events.h"
#include "snake.h"

// free the resources needed by the fruits
void kill_fruits(FRUITS* fruits) {
  int i;

  // iterate throgh each fruit and delete the window
  for(i = 0; i < fruits->length; i++) {
    delwin(fruits->fruits[i].win);
  }
  fruits->length = 0;
  // free the allocated memory
  free(fruits->fruits);
}

// remove a single fruit from the game by it's position
void kill_fruit(FRUITS *fruits, int posy, int posx) {
  FRUIT *fruit;
  // check if a fruit is on the given position
  if((fruit = fruit_is_on(fruits, posy, posx)) != NULL) {
    // remove the fruit
    kill_fruit_by_ptr(fruits, fruit);
  }
}
  
// remove a single fruit from the game
void kill_fruit_by_ptr(FRUITS *fruits, FRUIT *fruit) {
  int i;
  // check if FRUIT is in the array of FRUITS
  for(i = 0; i < fruits->length; i++) {
    if(&fruits->fruits[i] == fruit) {
      // remove the fruit
      kill_fruit_at_pos(fruits, i);
      break;
    }
  }
}

// remove a single fruit from the fruits array
void kill_fruit_at_pos(FRUITS *fruits, int i) {
  // move the frutis afterwards to front
  for(i++; i < fruits->length; i++) {
    fruits->fruits[i-1] = fruits->fruits[i];
  }

  // realloc lessmemory
  ruits->fruits = realloc(fruits->fruits, --fruits->length * sizeof(FRUIT));
}


// returns a FRUIT if one is found on the given point, else NULL
FRUIT *fruit_is_on(FRUITS *fruits, int posy, int posx) {
  int i;
  int y,x;

  // iterate each fruit
  for(i = 0; i < fruits->length; i++) {
    // read the position of the current fruit
    getbegyx(fruits->fruits[i].win, y, x);
    // check if the positions are matching
    if(posy == y && posx == x) {
      // return a pointer to the current fruit
      return &fruits->fruits[i];
    }
  }
  return NULL;
}

// create a new fruit in the game
void grow_fruit(GAME* game) {
  int y,x,randy,randx;
  // get the dimensions of the screen
  getmaxyx(stdscr, y, x);

  // generate a new random position until a empty spot is found
  do {
    randy = rand() % y;
    randx = rand() % x;
    // check if it is a empty spot
  } while (snake_part_is_on(&game->snake, randy, randx) != NULL || fruit_is_on(&game->fruits, randy, randx) != NULL || check_extended_border_collision(game, randy, randx));

  // allocate memory for the new fruit
  if(game->fruits.length == 0) {
    // initialize the array with malloc if it is the first fruit in the array
    game->fruits.fruits = malloc(sizeof(FRUIT) * ++game->fruits.length);
  } else {
    // allocate more memory
    game->fruits.fruits = realloc(game->fruits.fruits, sizeof(FRUIT) * ++game->fruits.length);
  }
  
  // get a filled struct of the new fruit
  get_fruit(&game->fruits.fruits[game->fruits.length - 1], randy, randx);
}


// fill in data into a fruit struct
void get_fruit(FRUIT *fruit, int posy, int posx) {
  // how the diffrent fruits are displayed
  static char chars[EFFECTS] = {'x', '@', '%'};
  // the diffrent effects of the fruits
  static void (*effects[EFFECTS])(GAME *) = {
    normal_effect,
    double_grow,
    mega_food
  };
  // the chance a certain fruit appears
  static int chance[EFFECTS] = {100, 25, 5};
  static int max_chance = 130;

  int i = 0;
  int sum = 0;
  // generate a random number
  int random = rand() % max_chance;
  // get the number of the fruit out of the random number
  while(random >= (sum += chance[i]) && i + 1 < EFFECTS) {
    i++;
  };
  // create a new window
  fruit->win = newwin(1, 1, posy, posx);
  // assign the effect
  fruit->effect = effects[i];
  // print the char on the window
  wprintw(fruit->win, "%c", chars[i]);
  // show the fruit
  wrefresh(fruit->win);
}

// redraw all the fruits on the screen
void redraw_fruits(FRUITS *fruits) {
  int i;

  // iterate through each fruit and refresh it.
  for(i = 0; i < fruits->length; i++) {
    redrawwin(fruits->fruits[i].win);
    wrefresh(fruits->fruits[i].win);
  }
}
