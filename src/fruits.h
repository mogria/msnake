#ifndef _FRUITS_H
#define _FRUITS_H

#include "types.h"
#include <stdlib.h>

void redraw_fruits(FRUITS *fruits);
void kill_fruits(FRUITS* fruits);
void kill_fruit(FRUITS *fruits, int posy, int posx);
WINDOW *fruit_is_on(FRUITS *fruits, int posy, int posx);
int grow_fruit(GAME* game);

#endif /* _FRUITS_H */
