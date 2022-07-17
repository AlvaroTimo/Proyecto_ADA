# Makefile for DSatur

EXEC=DSatur

HEADS=

OBJ=DSATUR.o

CPP=g++
OPTS=-O3 -Wall ${GFLAGS}

all: ${EXEC}

${EXEC}: ${OBJ}
	${CPP} ${OPTS} -o $@ ${OBJ}

%.o: %.cpp ${HEADS}
	${CPP} ${OPTS} -c -o $@ $<

clean:
	rm -f ${OBJ} ${EXEC}


