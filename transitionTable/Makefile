# Nom de l'exécutable
EXEC = Trie


SRCS = main.c


OBJS = $(SRCS:.c=.o)


CC = gcc


CFLAGS = -Wall -Wextra -g


all: $(EXEC)


$(EXEC): $(OBJS)
	$(CC) -o $@ $^


%.o: %.c structures.h
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f $(OBJS) $(EXEC)


mrproper: clean
	rm -f *~
