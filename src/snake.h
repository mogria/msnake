#ifndef SNAKE_H
#define SNAKE_H

#include "types.h"

void redraw_snake(SNAKE *snake);
void kill_snake(SNAKE *snake);
void grow_snake(SNAKE *snake, int posy, int posx);
int move_snake(GAME *game);
POINT *snake_part_is_on(SNAKE *snake, int posy, int posx);

#endif /* SNAKE_H */
