
#ifndef _FRUITS_H
#define _FRUITS_H

void kill_fruits(FRUITS* fruits);
void kill_fruit(FRUITS *fruits, int posy, int posx);
WINDOW *fruit_is_on(FRUITS *fruits, int posy, int posx);

#endif /* _FRUITS_H */
