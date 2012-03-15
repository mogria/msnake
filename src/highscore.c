#include "highscore.h"

// calculate the highscore out of the points and the time needed
int calculate_score(int points, long time_sec) {
  return points * 10 - time_sec * 2;
}


// read the highscore file
HIGHSCORE *read_highscore(int *num) {
  // static pointer to an array of highscore records
  static HIGHSCORE *ptr = NULL;
  // the highscore file handle
  FILE *hs_file;
  int x, swapped;
  // the current record
  HIGHSCORE current;

  // if a NULL pointer is given free all the allocated memory and exit
  if(num == NULL) {
    free(ptr);
    return NULL;
  }

  // open the highscore file
  if((hs_file = fopen(HIGHSCORE_FILE, "r")) == NULL) {
    return NULL;
  }

  // free the allocated memory of calls before
  if(ptr != NULL) {
    free(ptr);
  }

  // initialize the ptr and num
  *num = 0;
  ptr = malloc(*num);

  // read the records into the `ptr`-array
  while(fread(&current, sizeof(HIGHSCORE), 1, hs_file) != 0) {
    ptr = realloc(ptr, sizeof(HIGHSCORE) * ++(*num));
    ptr[*num - 1] = current;
  }

  // sort the whole thing using a bubble sort algorithm
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

  // return the array
  return ptr;
}

// add an entry to the highscore file
int add_highscore(char *name, int points, int time_sec) {
  FILE *hs_file;
  HIGHSCORE highscore;
  // write the values into the struct
  strncpy(highscore.name, name, HIGHSCORE_NAME_LENGTH - 1);
  highscore.points = points;
  highscore.time_sec = time_sec;
  highscore.highscore = calculate_score(points, time_sec);
  highscore.name[HIGHSCORE_NAME_LENGTH - 1] = '\0';

  // open the highscore fiel in append mode
  if((hs_file = fopen(HIGHSCORE_FILE, "a")) == NULL) {
    return 1;
  }
  // append a structure to the file
  fwrite(&highscore, sizeof(HIGHSCORE), 1, hs_file);
  // close the file
  fclose(hs_file);
  return 0;
}

// clear the highscore file
void clear_highscore() {
  FILE *hs_file = NULL;
  // open the file in write mode
  if((hs_file = fopen(HIGHSCORE_FILE, "w")) != NULL) {
    fclose(hs_file);
  }
}
