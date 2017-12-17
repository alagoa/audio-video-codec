P=Player
OBJECTS= Player.o VideoFile.o main.o
EBUG_OBJ= Player_debug.o VideoFile_debug.o main_debug.o
BOOSTFLAGS=-lboost_system -lboost_program_options -lboost_filesystem
CFLAGS = -Wall -O3 -g
CFLAGS_DEBUG = -Wall -g
LDLIBS=
OPENCV=`pkg-config opencv --cflags --libs`
CC=g++

all: $(P)
# Generate application
$(P): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(P) $(OBJECTS) $(OPENCV) $(BOOSTFLAGS)
# Generate object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o$@

debug: $(DEBUG_OBJ)
	$(CC) $(CFLAGS_DEBUG) -o $(P) $(DEBUG_OBJ)
%_debug.o: %.cpp
	$(CC) $(CFLAGS_DEBUG) -D DEBUG -c $< -o$@
clean:
	rm *.o $(P)
