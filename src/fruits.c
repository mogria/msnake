#include "fruits.h"
#include "effects.h"
#include "events.h"
#include "snake.h"

#include <stdlib.h>

// make sure to provide enough space to store
// all fruits when all fields contain a fruit
void manage_fruit_memory(GAME* game, int newsizey, int newsizex) {
  if(game->fruits.allocated != 0 && (
      game->rows == newsizey &&
      game->columns == newsizex
    )) {
    // fruit buffer already allocated and no changes in 
    // size of the playing field, so no adjustment needs
    // to be made
    return;
  }
  //fprintf(stderr, "%dx%d -> %dx%d\n", game->rows, game->columns, newsizey, newsizex);

  size_t max_num_fruits_to_allocate = newsizey * newsizex;

  // always allocate new and initialize with 0
  FRUIT* fruit_buffer = malloc(sizeof(FRUIT) * max_num_fruits_to_allocate);
  bzero(fruit_buffer, sizeof(FRUIT) * max_num_fruits_to_allocate);

  // in case we had a buffer before copy over the fruits
  // to the correct position so  they can be accessed
  // by index by multiplying their positions
  if(game->fruits.allocated != 0) {
      FRUIT* fruit = &game->fruits.fruits[0];
      for(int i = 0 ; i < max_num_fruits_to_allocate; i++) {
          fruit = &game->fruits.fruits[i];
          if (fruit->point.ch != '\0') {
            fruit_buffer[fruit->point.y * newsizey + fruit->point.x] = *fruit;
          }
      }

      // free the previous fruits buffer
      kill_fruits(&game->fruits);
  }

  game->fruits.fruits = fruit_buffer;
  fprintf(stderr, "fruits base ptr= %p, sizeof(FRUIT) = %lu\n", fruit_buffer, sizeof(FRUIT));

  game->fruits.allocated = max_num_fruits_to_allocate;
}


bool is_fruit_used(const FRUIT* fruit) {
    return fruit->point.ch != '\0';
}

FRUIT *get_fruit_addr(GAME* game, int y, int x) {
  return game->fruits.fruits + (y * game->rows) + x;
}

static inline void
draw_fruit(FRUIT *fruit)
{
  const POINT *point = &fruit->point;
  
  //clear space if not a fruit anymore
  if(!is_fruit_used(fruit)) {
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
  
  fruits->allocated = 0;
  fruits->used = 0;
  free(fruits->fruits);
}

// remove a single fruit from the game by it's position
void kill_fruit(FRUITS* fruits, FRUIT *fruit) {
  // mark fruit as unused again
  fruit->point.ch = '\0';
  if(fruits->used >= 1) fruits->used--;

  draw_fruit(fruit);
  // bzero after draw_fruit(), so
  // the x & y coorindates are now zeroe'd out
  bzero(fruit, sizeof(FRUIT));
}

// returns a FRUIT if one is found on the given point, else NULL
FRUIT *get_fruit_on(GAME *game, int posy, int posx) {
  FRUIT* fruit = get_fruit_addr(game, posy, posx);
  //fprintf(stderr, "get_fruit_on(%d, %d): fruit_addr=%p\n", posx, posy, fruit);
  if(fruit->point.ch == '\0') {
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


FRUIT* find_random_fruit_space(GAME * game) {
  // this is deliberately checked this way in order
  // to prevent an unsigned integer underflow
  const unsigned int used_spots = game->snake.length + game->num_walls;
  if (used_spots >= game->fruits.allocated ) {
    return NULL;
  }

  const unsigned int free_spots = game->fruits.allocated - used_spots;

  if (free_spots > (unsigned int)(game->fruits.allocated * 0.2)) {
    // in this case there can be found a new fruit with at least 10% chance
    int randy,randx;
    // generate a new random position until a empty spot is found
    do {
      randy = rand() % game->rows;
      randx = rand() % game->columns;

      // is nothing else there in the generated position?
    } while(!can_fruit_grow(game, randy, randx));

    FRUIT* new_fruit = NULL;
    new_fruit = get_fruit_addr(game, randy, randx);
    new_fruit->point.y = randy;
    new_fruit->point.x = randx;

    fprintf(stderr, "randomly picking fruit spot %dx%d\n", randy, randx);

    return new_fruit;
  } else {
    // now it gets more complicated, the random algorithm
    // gets real slow really fast, because it has to try a lot
    // harder to find an empty field
    //
    // Even worse when there is no empty field available the algorithm
    // never finishes, so let's make sure it does.


    // first find all free spots
    FRUIT** free_fruit_spots = malloc(sizeof(FRUIT*) * free_spots);
    unsigned int idx = 0;
    bool full = false;
    for(int y = 0; y < game->rows && !full; y++) {
      for(int x = 0; x < game->columns && !full; x++) {
        if (get_fruit_on(game, y, x) == NULL && can_fruit_grow(game, y, x)) {
          fprintf(stderr, "found free spot %dx%d", y, x);
          // that means no fruit is there :)
          FRUIT* free_fruit_spot = get_fruit_addr(game, y, x);

          // initialize fruit in case it's returned
          free_fruit_spot->point.y = y;
          free_fruit_spot->point.x = x;
          free_fruit_spots[idx] = free_fruit_spot;

          if (idx + 1 < free_spots) {
              idx++;
          } else {
              full = true;
          }
        }
      }
    }

    FRUIT* new_fruit = NULL;
    // select a random one
    unsigned int num_free_spots_found = idx + 1;
    new_fruit = free_fruit_spots[rand() % num_free_spots_found];

    fprintf(stderr, "selectively picking fruit spot %dx%d\n", new_fruit->point.y, new_fruit->point.x);
    free(free_fruit_spots);
    return new_fruit;
  }
}

// create a new fruit in the game
bool grow_fruit(GAME* game) {
  manage_fruit_memory(game, game->rows, game->columns);

  FRUIT* new_fruit = find_random_fruit_space(game);
  if (new_fruit == NULL) {
    return false;
  }

  // get a filled struct (containing the displayed char, the effect & co.) of the new fruit
  initialize_fruit(new_fruit);
  game->fruits.used++;
  draw_fruit(new_fruit);

  return true;

}

unsigned int grow_fruits(GAME* game, unsigned int amount) {
  for(int i = 0; i < FRUITS_INIT_NUMBER; i++) {
    grow_fruit(game);
  }
}


// fill in data into a fruit struct
void initialize_fruit(FRUIT *fruit) {
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
  //  important: this needs to be  the sum of all the chance's array values above
  static int max_chance = 132;

  int i = 0;
  int sum = 0;

  // generate a random number
  int random = rand() % max_chance;
  // Get the number of the fruit out of the random number by mapping it
  // onto the chances array.
  //
  // the index increase every time the random value is greater
  // than the sum of all elements before the index
  // and the element with the index
  // there is also a bounds check for the array
  while(random >= sum && i + 1 < EFFECTS) {
    sum += chance[i];
    i++; // next index
  };

  // create a new point
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
  // iterate through each fruit.
  for(int i = 0; i < fruits->allocated; i++) {
    // redraw it!
    draw_fruit(&fruits->fruits[i]);
  }
}
