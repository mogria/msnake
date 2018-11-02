#ifndef GAME_H
#define GAME_H

#include "types.h"
#include <time.h>
#include <signal.h>

void redraw_game(GAME *game);
void draw_border(GAME *game);
int pause_dialog();
void kill_game(GAME *game);
void display_highscore();
void run();

#endif /* GAME_H */
