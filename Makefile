.PHONY: all clean

CC 		:= gcc
CFLAGS	:= -std=gnu99 -pedantic -Wall

SRC		:= main.c measure.h measure.c

all: measure

measure: ${SRC}
	${CC} ${CFLAGS} main.c measure.c -o $@

clean:
	@rm -f measure
