#include "effects.h"
#include "fruits.h"

void normal_effect(GAME *game) {
  grow_fruit(game);
  game->snake.grow++;
  game->highscore++;
}

void double_grow(GAME *game) {
  normal_effect(game);
  grow_fruit(game);
}

void mega_food(GAME *game) {
  normal_effect(game);
  game->snake.grow += 4;
  game->highscore += 9;
}
