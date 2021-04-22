CFLAGS?=-O1 

CC=gcc

CFLAGS:=$(CFLAGS) -Wall -Wextra -Werror -fPIC

TARGETS=sender receiver 

UTILS=util.o

all: $(TARGETS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<

$(TARGETS): %:%.o util.o
	$(CC) $(CFLAGS) $^ -o $@

.PHONY:	clean

clean:
	rm -f *.o  $(TARGETS)