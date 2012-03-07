
LIBRARIES=-lcurses

all: clean
	[ ! -d bin ] && mkdir bin || true
	[ ! -d tmp ] && mkdir tmp || true
	gcc -c -o tmp/fruits.o src/fruits.c
	gcc -c -o tmp/game.o src/game.c
	gcc -c -o tmp/glog.o src/glog.c
	gcc -c -o tmp/snake.o src/snake.c
	gcc -c -o tmp/events.o src/events.c
	gcc -o bin/snake tmp/fruits.o tmp/game.o tmp/glog.o tmp/snake.o tmp/events.o src/main.c $(LIBRARIES)
	rm -rf tmp

clean:
	rm -rf bin

install:
	[ -f bin/snake ] && cp bin/snake /usr/games/bin/msnake
