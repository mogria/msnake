#include "main.h"
#include "dialog.h"
#include "highscore.h"
#include "game.h"

int main() {

  // for some better random numbers (and not always the same)
  srand(time(NULL));
  
  init_curses();

  main_menu();

  end_curses();

  // free the allocated memory for the highscore
  read_highscore(NULL);
  // close the logfile
  glog(NULL);
  return EXIT_SUCCESS;
}

void main_menu() {
  int selected_menu_entry;
  do {
    selected_menu_entry = display_menu();
    if(selected_menu_entry == 1) {
      // run the game
      run();
    } else if(selected_menu_entry == 2) {
      // display the highscores
      show_highscores();
    } else if(selected_menu_entry == 3) {
      // display a dialog which explains the controls of the game
      display_controls();
    } else if(selected_menu_entry == 4) {
      // display a dialog which explains the elements of the game
      display_help();
    } else if(selected_menu_entry == 5) {
      // clear highscores
      if(clear_score_dialog() == 1) {
          clear_highscore();
      }
    }
    // leave if the menu entry "exit" is chosen
  } while(selected_menu_entry != 6);
}

void init_curses() {

#if _BSD_SOURCE || _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
  // by default, ncurses stops the execution of the program for
  // 1 second if escape is pressed. Disable this behaviour.
  setenv("ESCDELAY", "0", 1);
#endif
  
  initscr();
  // get more control over the input
  cbreak();
  // getch() returns ERR if no input is present and doesn't wait
  nodelay(stdscr, TRUE);
  // don't echo the inserted keys to the screen
  noecho();
  // also grab keys like F1 etc.
  keypad(stdscr, TRUE);
}

void end_curses() {
  endwin();
}
