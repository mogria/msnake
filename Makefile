
all:
	gcc -o snake snake.c -lcurses

clean:
	rm -rf snake
