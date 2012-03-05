
all:
	[ ! -d bin ] && mkdir bin || true
	gcc -o bin/snake src/snake.c -lcurses -lrt

clean:
	rm -rf bin/snake
	rmdir bin
