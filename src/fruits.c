#include "fruits.h"
#include "snake.h"

void kill_fruits(FRUITS* fruits) {
  int i;

  for(i = 0; i < fruits->length; i++) {
    delwin(fruits->fruits[i]);
  }
  fruits->length = 0;
  free(fruits->fruits);
}

void kill_fruit(FRUITS *fruits, int posy, int posx) {
  WINDOW *fruit;
  int i;
  if((fruit = fruit_is_on(fruits, posy, posx)) != NULL) {
    for(i = 0; i < fruits->length; i++) {
      if(fruits->fruits[i] == fruit) {
        for(i++; i < fruits->length; i++) {
          fruits->fruits[i-1] = fruits->fruits[i];
        }
        fruits->fruits = realloc(fruits->fruits, --fruits->length * sizeof(WINDOW*));
        break;
      }
    }
  }
}
  

WINDOW *fruit_is_on(FRUITS *fruits, int posy, int posx) {
  int i;
  int y,x;

  for(i = 0; i < fruits->length; i++) {
    getbegyx(fruits->fruits[i], y, x);
    if(posy == y && posx == x) {
      return fruits->fruits[i];
    }
  }
  return NULL;
}

int grow_fruit(GAME* game) {
  int y,x,randy,randx;
  getmaxyx(stdscr, y, x);
  y--;
  x--;
  do {
    randy = (rand() % y) + 1;
    randx = (rand() % x) + 1;
  } while (snake_part_is_on(&game->snake, randy, randx) != NULL && fruit_is_on(&game->fruits, randy, randx) != NULL);

  if(game->fruits.length == 0) {
    game->fruits.fruits = malloc(sizeof(WINDOW*) * ++game->fruits.length);
  } else {
    game->fruits.fruits = realloc(game->fruits.fruits, sizeof(WINDOW*) * ++game->fruits.length);
  }
  
  game->fruits.fruits[game->fruits.length - 1] = newwin(1, 1, randy, randx);
  wprintw(game->fruits.fruits[game->fruits.length - 1], "%c", 'x');
  wrefresh(game->fruits.fruits[game->fruits.length - 1]);

}


