# Makefile for SimpleGreedy

EXEC=SimpleGreedy

HEADS=

OBJ=main.o

CPP=g++
OPTS=-O3 -Wall ${GFLAGS}

all: ${EXEC}

${EXEC}: ${OBJ}
	${CPP} ${OPTS} -o $@ ${OBJ}

%.o: %.cpp ${HEADS}
	${CPP} ${OPTS} -c -o $@ $<

clean:
	rm -f ${OBJ} ${EXEC}

