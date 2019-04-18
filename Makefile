CC=g++
CFLAGS=-lm -lncurses
DEPS = src/tetromino.h src/game.h
OBJ = src/main.cpp src/tetromino.cpp src/game.cpp
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
