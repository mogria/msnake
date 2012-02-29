
all:
	gcc -o snake snake.c -lcurses -lrt

clean:
	rm -rf snake
