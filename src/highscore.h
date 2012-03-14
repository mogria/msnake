#ifndef _HIGHSCORE_H
#define _HIGHSCORE_H

#include <stdio.h>
#include "types.h"

#define HIGHSCORE_FILE "msnake_scores.dat"

int calculate_score(int points, long time_sec);
HIGHSCORE *read_highscore(int *num);
int add_highscore(char *name, int points, int time_sec);
void clear_highscore();

#endif /* _HIGHSCORE_H */
