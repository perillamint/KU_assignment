LDFLAGS = 
CFLAGS = -Wall -Werror --std=gnu99 -ggdb -O0

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

procmon: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

all: procmon

clean:
	rm -f *.o procmon
