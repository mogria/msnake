#include "highscore.h"

// calculate the highscore out of the points and the time needed
int calculate_score(int points, long time_sec) {
  return points * 10 - time_sec / 2;
}


// read the highscore file
HIGHSCORE *read_highscore(int *num) {
  static HIGHSCORE *ptr = NULL;
  FILE *hs_file;
  int x, swapped;
  HIGHSCORE current;

  if(num == NULL) {
    free(ptr);
    return NULL;
  }

  if((hs_file = fopen(HIGHSCORE_FILE, "r")) == NULL) {
    return NULL;
  }

  if(ptr != NULL) {
    free(ptr);
  }

  *num = 0;
  ptr = malloc(*num);

  while(fread(&current, sizeof(HIGHSCORE), 1, hs_file) != 0) {
    ptr = realloc(ptr, sizeof(HIGHSCORE) * ++(*num));
    ptr[*num - 1] = current;
  }

  do {
    swapped = 0;
    for(x = 0; x < *num - 1; x++) {
      if(ptr[x].highscore < ptr[x + 1].highscore) {
        current = ptr[x];
        ptr[x] = ptr[x + 1];
        ptr[x + 1] = current;
        swapped = 1;
      }
    }
  } while (swapped);

  return ptr;
}

int add_highscore(char *name, int points, int time_sec) {
  FILE *hs_file;
  HIGHSCORE highscore;
  strncpy(highscore.name, name, HIGHSCORE_NAME_LENGTH - 1);
  highscore.points = points;
  highscore.time_sec = time_sec;
  highscore.highscore = calculate_score(points, time_sec);
  highscore.name[HIGHSCORE_NAME_LENGTH - 1] = '\0';

  if((hs_file = fopen(HIGHSCORE_FILE, "a")) == NULL) {
    return 1;
  }
  fwrite(&highscore, sizeof(HIGHSCORE), 1, hs_file);
  fclose(hs_file);
  return 0;
}

void clear_highscore() {
  FILE *hs_file = NULL;
  if((hs_file = fopen(HIGHSCORE_FILE, "w")) != NULL) {
    fclose(hs_file);
  }
}
