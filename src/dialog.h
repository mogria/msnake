#ifndef DIALOG_H
#define DIALOG_H
#include "types.h"
#include <curses.h>

#define DIALOG_WIDTH 46
#define DIALOG_HEIGHT 20
#define CONTENT_WIDTH (DIALOG_WIDTH - 2)
// \0 character
#define CONTENT_STR_WIDTH (CONTENT_WIDTH + 1)

WINDOW *create_dialog_window();
int create_numbered_dialog(const char *title, const char *contents, int lines);
int create_enter_dialog(const char *title, const char *contents, int lines);
void wait_return(WINDOW *win);
int display_menu();
void display_controls();
void display_help();
void display_highscore(GAME *game, char *buf, int length);
void show_highscores();
int pause_dialog();
int clear_score_dialog();
void enter_string(char *title, char *content, int posy, int posx, char *buf, int length);

#endif /* DIALOG_H */
