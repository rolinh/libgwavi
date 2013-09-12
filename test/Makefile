CC ?= gcc
MAKE ?= make
rm ?= rm

EXEC = test


CFLAGS = -O2 -std=c89 -fPIC ${INCLUDES}
LDFLAGS = -L${LIB} -lgwavi

INCLUDES=-I${INC} -I${TEST_INC}

# directories
INC=../inc
LIB=../lib
SRC=../src
OBJ=obj
TEST_INC=inc
UNIT=unit

UNITS = ${UNIT}/gwavi_test.c \
		${UNIT}/test_helper.c

OBJS = ${UNITS:${UNIT}/%.c=${OBJ}/%.o}

all: ${EXEC}
	./${EXEC}

${OBJS}: ${OBJ}/%.o : ${UNIT}/%.c
	${CC} ${CFLAGS} -o $@ -c $<

${EXEC}: ${OBJS}
	${CC} -o ${EXEC} ${OBJS} ${LDFLAGS} -Wl,-rpath=${LIB}

clean:
	${RM} -f ${OBJ}/*.o

mrproper: clean
	${RM} ${EXEC}

.PHONY: all clean mrproper
