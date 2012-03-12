#include "fruits.h"
#include "effects.h"
#include "events.h"
#include "snake.h"

void kill_fruits(FRUITS* fruits) {
  int i;

  for(i = 0; i < fruits->length; i++) {
    delwin(fruits->fruits[i].win);
  }
  fruits->length = 0;
  free(fruits->fruits);
}

void kill_fruit(FRUITS *fruits, int posy, int posx) {
  FRUIT *fruit;
  if((fruit = fruit_is_on(fruits, posy, posx)) != NULL) {
    kill_fruit_by_ptr(fruits, fruit);
  }
}
  
void kill_fruit_by_ptr(FRUITS *fruits, FRUIT *fruit) {
  int i;
  for(i = 0; i < fruits->length; i++) {
    if(&fruits->fruits[i] == fruit) {
      kill_fruit_at_pos(fruits, i);
      break;
    }
  }
}

void kill_fruit_at_pos(FRUITS *fruits, int i) {
  for(i++; i < fruits->length; i++) {
    fruits->fruits[i-1] = fruits->fruits[i];
  }
  fruits->fruits = realloc(fruits->fruits, --fruits->length * sizeof(FRUIT));
}

FRUIT *fruit_is_on(FRUITS *fruits, int posy, int posx) {
  int i;
  int y,x;

  for(i = 0; i < fruits->length; i++) {
    getbegyx(fruits->fruits[i].win, y, x);
    if(posy == y && posx == x) {
      return &fruits->fruits[i];
    }
  }
  return NULL;
}

void grow_fruit(GAME* game) {
  int y,x,randy,randx;
  getmaxyx(stdscr, y, x);
  y--;
  x--;
  do {
    randy = rand() % (y + 1);
    randx = rand() % (x + 1);
  } while (snake_part_is_on(&game->snake, randy, randx) != NULL || fruit_is_on(&game->fruits, randy, randx) != NULL || check_extended_border_collision(game, randy, randx));

  if(game->fruits.length == 0) {
    game->fruits.fruits = malloc(sizeof(FRUIT) * ++game->fruits.length);
  } else {
    game->fruits.fruits = realloc(game->fruits.fruits, sizeof(FRUIT) * ++game->fruits.length);
  }
  
  get_fruit(&game->fruits.fruits[game->fruits.length - 1], randy, randx);
}


void get_fruit(FRUIT *fruit, int posy, int posx) {
  static char chars[EFFECTS] = {'x', '@', '%'};
  static void (*effects[EFFECTS])(GAME *) = {
    normal_effect,
    double_grow,
    mega_food
  };
  static int chance[EFFECTS] = {100, 25, 5};
  static int max_chance = 130;

  int i = 0;
  int sum = 0;
  int random = rand() % max_chance + 1;
  while(random > (sum += chance[i]) && i + 1 < EFFECTS) {
    i++;
  };
  fruit->win = newwin(1, 1, posy, posx);
  fruit->effect = effects[i];
  wprintw(fruit->win, "%c", chars[i]);
  wrefresh(fruit->win);
}

void redraw_fruits(FRUITS *fruits) {
  int i;

  for(i = 0; i < fruits->length; i++) {
    redrawwin(fruits->fruits[i].win);
    wrefresh(fruits->fruits[i].win);
  }
}
