CC=gcc                              # překladač jazyka C
CFLAGS=-std=c99 -Werror -Wextra -Wall -DNDebug  # parametry překladače

main: main.c
	$(CC) $(CFLAGS) main.c -o main -lm                                  
