CC = gcc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = 
EXEC = stnc
SRC = stnc.c
OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
