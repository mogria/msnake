#include "main.h"
#include "dialog.h"
#include "game.h"

int main() {
  int action;
  // for some better random numbers
  srand(time(NULL));
  
  // start the curses mode
  initscr();
  //getch returns ERR if no input is present and doesn't wait
  nodelay(stdscr, TRUE);
  //don't echo the inserted keys to the screen
  noecho();
  //also grab keys like F1 etc.
  keypad(stdscr, TRUE);

  do {
    // display the menu
    action = display_menu();
    if(action == 1) {
      // run the game
      run();
    } else if(action == 2) {
      // display a dialog which explains the controls of the game
      display_controls();
    } else if(action == 3) {
      // display a dialog which explains the elements of the game
      display_help();
    }
    // leave if the menu entry "exit" is chosen
  } while(action != 4);

  // end the curses mode
  endwin();
  
  // close the logfile
  glog(NULL);
  return EXIT_SUCCESS;
}
