#ifndef _MAIN_H
#define _MAIN_H
#include <stdlib.h>
#include "types.h"

WINDOW *create_dialog();
void wait_return();
int display_menu();
void display_controls();
void display_help();
void display_highscore(GAME *game);

#endif /* _MAIN_H */
