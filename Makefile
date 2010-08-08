CC=gcc
CFLAGS=`sdl-config --cflags` -Wall -ggdb
LIBS=`sdl-config --libs` -lGL -lGLU -lm

all:
	gcc ${CFLAGS} -c main.c
	gcc ${CFLAGS} -c memory.c
	gcc ${CFLAGS} -c hle_game.c
	gcc ${CFLAGS} -c hle_entity.c
	gcc ${CFLAGS} -c hle_player.c
	gcc ${CFLAGS} -c strlcpy.c
	gcc ${CFLAGS} -c objloader.c
	gcc ${CFLAGS} -c fatal.c
	gcc ${LIBS} -o hivexplorer main.o hle_game.o hle_entity.o hle_player.o \
		memory.o strlcpy.o objloader.o fatal.o


