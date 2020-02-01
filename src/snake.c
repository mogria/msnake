#include "snake.h"
#include "events.h"

// prints the snake part character
static inline void
draw_part(POINT *part)
{
  // set the color and brightness
  attron(COLOR_PAIR(2));
  // print the char on the main window
  mvprintw(part->y, part->x, "%c", part->ch);
}

// check if a part of the snake is on the given spot
// returns a POINT * if one is found, NULL if not
POINT *snake_part_is_on(SNAKE *snake, int posy, int posx) {
  int i;
  POINT *point; // position of the current snake part we are iterating

  // iterate each part
  for(i = 0; i < snake->length; i++) {
    // get the position of the current part
    point = &snake->parts[i];

    // compare the position
    if(point->y == posy && point->x == posx) {
      // return the current part
      return point;
    }
  }

  return NULL;
}

// increase the length of the name by one
void grow_snake(SNAKE *snake, int posy, int posx) {
  // a point containing a single char of a snake
  POINT *point;

  // increase the length of the snake
  snake->length++;
  // allocate memory for the new part
  if(snake->allocated == 0) {
    // initialize the array with malloc if it is the first part
    snake->parts = malloc(sizeof(POINT) * snake->length);
    snake->allocated = snake->length;
  } else if(snake->allocated < snake->length) {
	// increase the amount of allocated memory
    snake->allocated *= 2;
    snake->parts = realloc(snake->parts, sizeof(POINT) * snake->allocated);
  }
  
  // create a new point
  point = &snake->parts[snake->length - 1];
  point->x = posx;
  point->y = posy;
  // print the character on the window
  point->ch = 'O';

  // display the part
  draw_part(point);

  return;
}

// free the allocated memory for the snake
void kill_snake(SNAKE *snake) {
  // free the resources allocated for the window pointers
  free(snake->parts);
}

// move the snake
int move_snake(GAME *game) {
  int success = 1, i;

  // some variables containing positions of certain things
  int newx, newy, lasty, lastx;

  // callbacks to check collisions
  int (*collision_checks[EVENTS])(GAME*, int, int) = {
    check_self_collision,
    check_extended_border_collision,
    check_fruit_collision
  };

  // callbacks which will be called automaticly if a collision is detected
  // the callback at position 1 in the array belongs to the check-function
  // at position 1 from the array above
  int (*collision_handlers[EVENTS])(GAME*, int, int) = {
    check_self_collision_handler,
    check_border_collision_handler,
    check_fruit_collision_handler
  };


  // difference on x-axis according to the direction
  int xdiff = game->snake.dir == DIR_LEFT ? -1 : (game->snake.dir == DIR_RIGHT ? 1 : 0);
  // difference on y-axis according to the direction
  int ydiff = game->snake.dir == DIR_UP ? -1 : (game->snake.dir == DIR_DOWN ? 1 : 0);

  // the position of the snake head
  newy = game->snake.parts[0].y;
  newx = game->snake.parts[0].x;

  // make a copy
  lasty = newy;
  lastx = newx;

  // calculate the new position and prevent from exceeding the size of the screen
  newy = (newy + ydiff) % game->rows;
  newx = (newx + xdiff) % game->columns;
  // the values have to be positive
  newy = newy < 0 ? game->rows + newy : newy;
  newx = newx < 0 ? game->columns + newx : newx;

  // check for collisons and execute the handlers if a collision occured
  for(i = 0; i < EVENTS && success; i++) {
    // collision?
    if((collision_checks[i](game, newy, newx))) {
      // should we end the game because of the collision?
      if(!collision_handlers[i](game, newy, newx)) {
        success = 0;
      }
    }
  }

  // no collisions ?
  if(success) {
    // iterate through each part of the snake
    for(i = 0; i < game->snake.length; i++) {
      if(i == 0) {
        // changes the direction of the head
        game->snake.parts[i].ch = (char) game->snake.dir;
      }
      // move the part to the position of the previous one
      lastx = game->snake.parts[i].x;
      lasty = game->snake.parts[i].y;

      game->snake.parts[i].x = newx;
      game->snake.parts[i].y = newy;

      // make a copy
      newy = lasty;
      newx = lastx;
    }

    // grow?
    if(game->snake.grow > 0) {
      // grow the snake
      grow_snake(&game->snake, lasty, lastx);

      // decrease the grow counter (number of times the snake will grow in the future)
      game->snake.grow--;
    } else {
      // is the snake head on the same position as the last part of the snake was before?
      if(lasty != game->snake.parts[0].y || lastx != game->snake.parts[0].x) {
        POINT empty_point = {lastx, lasty, ' '};
        // if no print a space at this position
        draw_part(&empty_point);
      }
    }

	// draw the head
	draw_part(&game->snake.parts[0]);

    // redraw the second part, it changes from head to O
    if (game->snake.length > 1)
      draw_part(&game->snake.parts[1]);
  }
  return success;
}

// redraw the whole snake
void redraw_snake(SNAKE *snake) {
  POINT *point;
  int i;

  // iterate through each part of the snake
  for(i = 0; i < snake->length; i++) {
    // redraw the current part
    point = &snake->parts[i];
    draw_part(point);
  }
}
