#include "glog.h"

void glog(const char *format, ... ) {
  va_list args;
  static FILE *logfile = NULL;

  char *fformat;
  int length = strlen(format) + 2;
 
  if(logfile == NULL) {
    logfile = fopen(LOG_FILE, "w");
    if(logfile == NULL) {
      fprintf(stderr, "Cannot open logfile %s\n", LOG_FILE);
      exit(EXIT_FAILURE);
    }
  }

  if(format != NULL) {
    fformat = malloc(sizeof(char) * length);
    strncpy(fformat, format, length - 2);
    fformat[length - 2] = '\n';
    fformat[length - 1] = '\0';

    va_start(args, format);
    vfprintf(logfile, fformat, args);
    fflush(logfile);
    free(fformat);
    va_end(args);
  } else {
    fclose(logfile);
  }

}
