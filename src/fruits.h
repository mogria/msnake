#ifndef FRUITS_H
#define FRUITS_H

// an initialization number of fruits
#define FRUITS_INIT_NUMBER (50)

#include "types.h"
#include <stdbool.h>
#include <stdlib.h>

void manage_fruit_memory(GAME* game, int newsizey, int newsizex);
bool is_fruit_used(const FRUIT* fruit);
void initialize_fruit(FRUIT *fruit);
void redraw_fruits(FRUITS *fruits);
void kill_fruits(FRUITS* fruits);
void kill_fruit(FRUITS* fruits, FRUIT *fruit);
FRUIT *get_fruit_on(GAME  *fruits, int posy, int posx);
bool grow_fruit(GAME* game);
unsigned int grow_fruits(GAME* game, unsigned int amount);

#endif /* FRUITS_H */
