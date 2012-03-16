#include "status-display.h"
#include "highscore.h"

void status_display(GAME *game) {
  int sx, sy;
  getmaxyx(stdscr, sy, sx);
  const int width = 25;
  WINDOW *win = newwin(1, width, 0, sx - width);
  wprintw(win,"%03iL-%05iP-%04is-%04iSCR",
      game->snake.length,
      game->highscore,
      time(NULL) - game->started - game->paused,
      calculate_score(game->highscore, time(NULL) - game->started - game->paused));
  wrefresh(win);
}
