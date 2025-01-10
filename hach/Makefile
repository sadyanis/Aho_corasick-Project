
EXEC = Trie


SRCS = hash.c


OBJS = $(SRCS:.c=.o)


CC = gcc


CFLAGS = -Wall -Wextra -g


all: $(EXEC)


$(EXEC): $(OBJS)
	$(CC) -o $@ $^


%.o: %.c structureHash.h
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f $(OBJS) $(EXEC)


mrproper: clean
	rm -f *~
