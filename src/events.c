#include "events.h"
#include "snake.h"
#include "fruits.h"

int check_fruit_collision(GAME* game, int cury, int curx) {
  return fruit_is_on(&game->fruits, cury, curx) != NULL;
}
 
int check_fruit_collision_handler(GAME* game, int cury, int curx) {
  kill_fruit(&game->fruits, cury, curx);
  grow_fruit(game);
  game->snake.grow++;
  game->highscore++;
  glog("fruit collision");
  return 1;
}

int check_border_collision(GAME* game, int cury, int curx) {
  int y,x,back;
  getmaxyx(stdscr, y, x);
  y--;
  x--;
  return cury <= 0 || curx <= 0 || cury >= y || curx >= x;
}

int check_border_collision_handler(GAME* game, int cury, int curx) {
  glog("border collision %i %i", cury, curx);
  return 0;
}

int check_self_collision(GAME* game, int cury, int curx) {
  int back = 0;
  WINDOW* on;
  glog("SNAKE PART ON %i %i", cury, curx);
  back = ! ((on = snake_part_is_on(&game->snake, cury, curx)) == NULL || on == game->snake.parts[game->snake.length - 1]);
  glog("returning %i", back);
  return back;
}

int check_self_collision_handler(GAME* game, int cury, int curx) {
  glog("self collision");
  return 0;
}
