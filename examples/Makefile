CC ?= gcc
MAKE ?= make
rm ?= rm

EXEC = demo

CFLAGS = -O2 -std=c89 -fPIC ${INCLUDES}
LDFLAGS = -L${LIB} -lgwavi
CFDEBUG = -O0 -g3 -pedantic -Wall -Wextra -Wconversion -Wstrict-prototypes \
		  -Wcast-qual -Wcast-align -Wshadow -Wredundant-decls -Wundef \
		  -Wfloat-equal -Wmissing-include-dirs -Wswitch-default -Wswitch-enum \
		  -Wpointer-arith -Wbad-function-cast -Wnested-externs \
		  -Wold-style-definition -Wsign-conversion -Wlogical-op \
		  -Wno-long-long -pipe -Wunreachable-code

INCLUDES=-I${INC}

INC = ../inc
LIB = ../lib
OBJ = obj
SRC = src

SRCS = ${SRC}/demo.c

OBJS = ${SRCS:${SRC}/%.c=${OBJ}/%.o}

.PATH: ${SRC}

all: ${EXEC}

${OBJS}: ${OBJ}/%.o : ${SRC}/%.c
	${CC} ${CFLAGS} -o $@ -c $<

${EXEC}: ${OBJS}
	${CC} -o ${EXEC} ${OBJS} ${LDFLAGS} -Wl,-rpath=${LIB}

debug: ${EXEC}
debug: CFLAGS += ${CFDEBUG}

clean:
	${RM} -f ${OBJ}/*.o

mrproper: clean
	${RM} ${EXEC}

.PHONY: all clean debug mrproper
