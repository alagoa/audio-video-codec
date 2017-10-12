P=FCM
OBJECTS= FCM.o main.o
DEBUG_OBJ= FCM_debug.o main_debug.o
BOOSTFLAGS=-lboost_serialization
CFLAGS = -Wall -O3
CFLAGS_DEBUG = -Wall -g
LDLIBS=
CC=g++

all: $(P)
# Generate application
$(P): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(P) $(BOOSTFLAGS) $(OBJECTS)
# Generate object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o$@

debug: $(DEBUG_OBJ)
	$(CC) $(CFLAGS_DEBUG) -o $(P) $(DEBUG_OBJ)
%_debug.o: %.cpp
	$(CC) $(CFLAGS_DEBUG) -D DEBUG -c $< -o$@
clean:
	rm *.o $(P)
