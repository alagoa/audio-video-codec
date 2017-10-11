P=FCM
OBJECTS= FCM.o main.o
CFLAGS = -Wall -O3
LDLIBS=
CC=g++

all: $(P)
# Generate application
$(P): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(P) $(OBJECTS)
# Generate object files
%.o: %.cpp
	$(CC) $(CFLAGS) -D DEBUG -c $< -o$@
clean:
	rm *.o $(P)
