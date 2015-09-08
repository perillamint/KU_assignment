LDFLAGS = 
CFLAGS = -Wall -Werror --std=c99 -ggdb

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

queuesim: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

all: queuesim

clean:
	rm -f *.o queuesim
