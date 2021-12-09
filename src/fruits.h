#ifndef FRUITS_H
#define FRUITS_H

#include "types.h"
#include <stdlib.h>

void manage_fruit_memory(GAME* game);
void initialize_fruit(FRUIT *fruit, int posy, int posx);
void redraw_fruits(GAME *game);
void kill_fruits(FRUITS* fruits);
void kill_fruit(FRUIT *fruit);
FRUIT *get_fruit_on(GAME  *fruits, int posy, int posx);
void grow_fruit(GAME* game);

#endif /* FRUITS_H */
