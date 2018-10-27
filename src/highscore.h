#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <stdio.h>
#include "types.h"

#define HIGHSCORE_DIR ".msnake"
#define HIGHSCORE_FILE "highscores.dat"

int calculate_score(int points, long time_sec);
HIGHSCORE *read_highscore(int *num);
int add_highscore(char *name, int points, int time_sec);
void clear_highscore();

#endif /* HIGHSCORE_H */
