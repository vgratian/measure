.PHONY: all clean

CC		:= gcc
CFLAGS	:= -std=gnu99 -pedantic -Wall

SRC		:= measure.c print.c
HDR		:= measure.h print.h

all: measure

measure: main.c ${SRC} ${HDR}
	${CC} ${CFLAGS} main.c ${SRC} -o $@

clean:
	rm -fv measure
