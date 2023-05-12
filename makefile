CC = gcc
CFLAGS = -Wall -Wextra -pedantic -pthread
OBJ = stnc.o performance_test.o performance_server.o

all: stnc performance_test performance_server

stnc: stnc.o
	$(CC) $(CFLAGS) stnc.o -o stnc

performance_test: performance_test.o
	$(CC) $(CFLAGS) performance_test.o -o performance_test

performance_server: performance_server.o
	$(CC) $(CFLAGS) performance_server.o -o performance_server

stnc.o: stnc.c stnc.h
	$(CC) $(CFLAGS) -c stnc.c -o stnc.o

performance_test.o: performance_test.c
	$(CC) $(CFLAGS) -c performance_test.c -o performance_test.o

performance_server.o: performance_server.c
	$(CC) $(CFLAGS) -c performance_server.c -o performance_server.o

clean:
	rm -f $(OBJ) stnc performance_test performance_server
