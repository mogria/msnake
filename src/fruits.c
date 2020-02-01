#include "fruits.h"
#include "effects.h"
#include "events.h"
#include "snake.h"

// prints the fruit character
static inline void
draw_fruit(FRUIT *fruit)
{
  POINT *point = &fruit->point;
  // set the color and brightness
  attron(A_BOLD | COLOR_PAIR(fruit->color));
  // print the char on the main window
  mvprintw(point->y, point->x, "%c", point->ch);
}

// free the resources needed by the fruits
void kill_fruits(FRUITS* fruits) {
  // reset the length
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
}


// returns a FRUIT if one is found on the given point, else NULL
FRUIT *fruit_is_on(FRUITS *fruits, int posy, int posx) {
  POINT *point;
  int i;

  // iterate each fruit
  for(i = 0; i < fruits->length; i++) {
    // read the position of the current fruit
    point = &fruits->fruits[i].point;
    // check if the positions are matching
    if(point->y == posy && point->x == posx) {
      // return a pointer to the current fruit
      return &fruits->fruits[i];
    }
  }
  return NULL;
}

// create a new fruit in the game
void grow_fruit(GAME* game) {
  int randy,randx;
  // generate a new random position until a empty spot is found
  do {
    randy = rand() % game->rows;
    randx = rand() % game->columns;
    // is nothing else there in the generated position?
  } while (snake_part_is_on(&game->snake, randy, randx) != NULL || fruit_is_on(&game->fruits, randy, randx) != NULL || check_extended_border_collision(game, randy, randx));

  game->fruits.length++;
  // allocate memory for the new fruit
  if(game->fruits.allocated == 0) {
    // initialize the array with malloc if it is the first fruit in the array
    game->fruits.fruits = malloc(sizeof(FRUIT) * game->fruits.length);
    game->fruits.allocated = game->fruits.length;
  } else if(game->fruits.allocated < game->fruits.length) {
    // allocate more memory
    game->fruits.allocated *= 2;
    game->fruits.fruits = realloc(game->fruits.fruits, sizeof(FRUIT) * game->fruits.allocated);
  }

  // get a filled struct (containing the displayed char, the effect & co.) of the new fruit
  get_fruit(&game->fruits.fruits[game->fruits.length - 1], randy, randx);
}


// fill in data into a fruit struct
void get_fruit(FRUIT *fruit, int posy, int posx) {
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
void redraw_fruits(FRUITS *fruits) {
  int i;

  // iterate through each fruit.
  for(i = 0; i < fruits->length; i++) {
    // redraw it!
    draw_fruit(&fruits->fruits[i]);
  }
}
