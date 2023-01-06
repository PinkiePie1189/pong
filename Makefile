CC=g++
LFLAGS=-lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf 
pong: main.cpp
	$(CC) $< -Iinclude -o $@ $(LFLAGS)
