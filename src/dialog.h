#ifndef _DIALOG_H
#define _DIALOG_H
#include "types.h"
#include <curses.h>
WINDOW *create_dialog();
void wait_return(WINDOW *win);
int display_menu();
void display_controls();
void display_help();
void display_highscore(GAME *game);

#endif /* _DIALOG_H */
