CFLAGS=-Wall -std=c11 -g
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

scc: $(OBJS)
	$(CC) -o scc $(OBJS) $(LDFLAGS)

$(OBJS): scc.h

test: scc
	./scc -test
	./test.sh

clean:
	rm -f scc *.o *~ tmp*
