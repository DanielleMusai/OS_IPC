CC = gcc
CFLAGS = -Wall -Wextra -pedantic -pthread
OBJ = stnc.o performance_client.o performance_server.o

.PHONY: all clean

all: stnc performance_client performance_server

stnc: stnc.o
	$(CC) $(CFLAGS) $^ -o $@

performance_client: performance_client.o
	$(CC) $(CFLAGS) $^ -o $@

performance_server: performance_server.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) stnc performance_client performance_server
