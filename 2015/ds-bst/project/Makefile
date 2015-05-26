LDFLAGS = 
CFLAGS = -Wall -Werror --std=c99 -ggdb -O0

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

integerapp: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

all: integerapp

clean:
	rm -f *.o integerapp
