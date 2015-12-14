#ifndef _HIGHSCORE_H
#define _HIGHSCORE_H

#include <stdio.h>
#include "types.h"

#define HIGHSCORE_FILE "/.msnake/highscores.dat"

char* user_home;
char* hscore_file_path;
size_t hscore_path_len;
char* hscore_file;

int calculate_score(int points, long time_sec);
HIGHSCORE *read_highscore(int *num);
int add_highscore(char *name, int points, int time_sec);
void clear_highscore();

#endif /* _HIGHSCORE_H */
