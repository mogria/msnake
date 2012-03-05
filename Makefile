
all:
	[ ! -d bin ] && mkdir bin
	gcc -o bin/snake src/snake.c -lcurses -lrt

clean:
	rm -rf bin/snake
	rmdir bin
