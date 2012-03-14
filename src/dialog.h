#ifndef _DIALOG_H
#define _DIALOG_H
#include "types.h"
#include <curses.h>

#define DIALOG_WIDTH 40
#define DIALOG_HEIGHT 20
#define CONTENT_WIDTH (DIALOG_WIDTH - 2)
WINDOW *create_dialog_window();
int create_numbered_dialog(const char *title, const char *contents, int lines);
int create_enter_dialog(const char *title, const char *contents, int lines);
void wait_return(WINDOW *win);
int display_menu();
void display_controls();
void display_help();
void display_highscore(GAME *game);
void show_highscores();
int pause_dialog();

#endif /* _DIALOG_H */
