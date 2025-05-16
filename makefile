
CC = gcc
CFLAGS = -g -Wextra -Wall

build: myserver.c
	$(CC) $(CFLAGS) -o myserver myserver.c 
