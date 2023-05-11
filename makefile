CC = gcc
CFLAGS = -Wall -Wextra -pedantic -pthread
OBJ = stnc.o performance_client.o performance_server.o

all: stnc performance_client performance_server

stnc: stnc.o
	$(CC) $(CFLAGS) stnc.o -o stnc

performance_client: performance_client.o
	$(CC) $(CFLAGS) performance_client.o -o performance_client

performance_server: performance_server.o
	$(CC) $(CFLAGS) performance_server.o -o performance_server

stnc.o: stnc.c stnc.h
	$(CC) $(CFLAGS) -c stnc.c -o stnc.o

performance_client.o: performance_client.c
	$(CC) $(CFLAGS) -c performance_client.c -o performance_client.o

performance_server.o: performance_server.c
	$(CC) $(CFLAGS) -c performance_server.c -o performance_server.o

clean:
	rm -f $(OBJ) stnc performance_client performance_server
