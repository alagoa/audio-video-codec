P=FakeAudioCodec
OBJECTS= Golomb.o AudioReader.o Predictor.o main.o
DEBUG_OBJ= Golomb_debug.o AudioReader_debug.o Predictor_debug.o main_debug.o
# BOOSTFLAGS=-lboost_system -lboost_serialization -lboost_program_options -lboost_filesystem
CFLAGS = -Wall -O3 -g
CFLAGS_DEBUG = -Wall -g
LDLIBS=
CC=g++

all: $(P)
# Generate application
$(P): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(P) -lsndfile $(OBJECTS)
# Generate object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o$@

debug: $(DEBUG_OBJ)
	$(CC) $(CFLAGS_DEBUG) -o $(P) -lsndfile $(DEBUG_OBJ)
%_debug.o: %.cpp
	$(CC) $(CFLAGS_DEBUG) -D DEBUG -c $< -o$@
clean:
	rm *.o $(P)
