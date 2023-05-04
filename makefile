CC = gcc
CFLAGS = -Wall -Wextra -pedantic -pthread
OBJ = stnc.o

all: stnc

stnc: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o stnc

stnc.o: stnc.c stnc.h
	$(CC) $(CFLAGS) -c stnc.c -o stnc.o

clean:
	rm -f $(OBJ) stnc
