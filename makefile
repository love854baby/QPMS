CC = gcc
OBJECTS = Main.o QPMS.o
CFLAGS = -c -Wall

all: $(OBJECTS)
	$(CC) $(OBJECTS) -lm -g3

%.o: %.c
	$(CC) $(CFLAGS) $<

clean: $(OBJECTS) a.out
	rm *.o a.out
