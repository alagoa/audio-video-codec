P=lossless_encoder
OBJECTS= jpeg_ls.o jpeg_ls_decoder.o main.o
DEBUG_OBJ= jpeg_ls_debug.o jpeg_ls_decoder.o main_debug.o
BOOSTFLAGS=-lboost_program_options
CFLAGS = -Wall -O3 -g
CFLAGS_DEBUG = -Wall -g
LDLIBS=
OPENCV=`pkg-config opencv --cflags --libs`
CC=g++

all: $(P)
# Generate application
$(P): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(P) $(OBJECTS) $(OPENCV)
# Generate object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o$@

debug: $(DEBUG_OBJ)
	$(CC) $(CFLAGS_DEBUG) -o $(P) $(DEBUG_OBJ) $(OPENCV)
%_debug.o: %.cpp
	$(CC) $(CFLAGS_DEBUG) -D DEBUG -c $< -o$@
clean:
	rm *.o $(P)
