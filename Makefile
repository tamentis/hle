CC?=gcc
CFLAGS+=`sdl-config --cflags` -Wall -ggdb
LIBS+=`sdl-config --libs` -lGL -lGLU -lm
OBJECTS=main.o hle_app.o hle_entity.o hle_player.o \
	memory.o strlcpy.o objloader.o fatal.o hle_land.o

PROGRAM=hle

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $(PROGRAM)

$(OBJECTS): %.o: %.c hive.h
	$(CC) ${CFLAGS} -c $<

clean:
	rm -f $(OBJECTS) $(PROGRAM) tags TAGS LOG

tags:
	etags *.c *.h

memwatch:
	top -d 0.1 -p `pidof rezerwar`

cpuwatch:
	top -d 1.0 -p `pidof rezerwar`

profiler:
	valgrind --tool=callgrind ./${PROGRAM}

valgrind:
	valgrind --leak-check=full --show-reachable=yes ./$(PROGRAM) >& LOG
