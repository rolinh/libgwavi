VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_PATCH = 0
VERSION = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
NAME = gwavi

CC ?= gcc
DOXYGEN ?= doxygen
LN ?= ln
MAKE ?= make
rm ?= rm
cp ?= cp

CFLAGS = -O3 -std=c89 -ggdb3 -fPIC ${INCLUDES}
CFDEBUG = -O0 -g3 -pedantic -Wall -Wextra -Wconversion -Wstrict-prototypes \
		  -Wcast-qual -Wcast-align -Wshadow -Wredundant-decls -Wundef \
		  -Wfloat-equal -Wmissing-include-dirs -Wswitch-default -Wswitch-enum \
		  -Wpointer-arith -Wbad-function-cast -Wnested-externs \
		  -Wold-style-definition -Wsign-conversion -Wlogical-op \
		  -Wno-long-long -pipe -Wunreachable-code

INCLUDES=-I${INC}

DOC = doc
EXAMPLES = examples
INC = inc
LIB = lib
OBJ = obj
SRC = src
TEST= test

SRCS = ${SRC}/avi-utils.c \
	   ${SRC}/gwavi.c \
	   ${SRC}/fileio.c

OBJS = ${SRCS:${SRC}/%.c=${OBJ}/%.o}

.PATH: ${SRC}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} -Wl,-soname,lib${NAME}0.so.${VERSION_MAJOR} -shared -o ${LIB}/lib${NAME}0.so.${VERSION} ${OBJS}
	${LN} -sf lib${NAME}0.so.${VERSION} ${LIB}/lib${NAME}0.so.${VERSION_MAJOR}
	${LN} -sf lib${NAME}0.so.${VERSION} ${LIB}/lib${NAME}0.so.${VERSION_MAJOR}.${VERSION_MINOR}
	${LN} -sf lib${NAME}0.so.${VERSION} ${LIB}/lib${NAME}0.so

${OBJS}: ${OBJ}/%.o : ${SRC}/%.c
	${CC} ${CFLAGS} -o $@ -c $<

all: ${NAME} examples doc

install: all
	rm -rf ${DESTDIR}${PREFIX}/usr/lib ${DESTDIR}${PREFIX}/usr/include
	mkdir -p ${DESTDIR}${PREFIX}/usr/lib ${DESTDIR}${PREFIX}/usr/include
	cp -a lib/libgwavi* ${DESTDIR}${PREFIX}/usr/lib
	chmod 644 ${DESTDIR}${PREFIX}/usr/lib/libgwavi*
	install -Dm644 inc/*.h ${DESTDIR}${PREFIX}/usr/include

debug: ${NAME}

debug: CFLAGS += ${CFDEBUG}

examples: ${NAME}
	${MAKE} -C ${EXAMPLES}

doc: ${OBJS}
	${DOXYGEN} ${DOC}/Doxyfile

#test: ${NAME}
#	${MAKE} -C ${TEST}

clean:
	${RM} -f ${OBJ}/*.o
	${MAKE} -C ${EXAMPLES} clean
	${MAKE} -C ${TEST} clean

mrproper: clean
	${RM} -rf ${LIB}/*.so* ${DOC}/html ${DOC}/latex ${DOC}/man
	${MAKE} -C ${EXAMPLES} mrproper
	${MAKE} -C ${TEST} mrproper

package: clean
	mkdir libgwavi-$(VERSION)
	cp -a AUTHORS.md debian doc examples inc lib LICENSE Makefile obj README.md src test libgwavi-$(VERSION)
	rm -rf libgwavi-$(VERSION)/doc/html
	tar czf ../libgwavi_$(VERSION).orig.tar.gz libgwavi-$(VERSION)
	rm -rf libgwavi-$(VERSION)

.PHONY: all clean debug doc examples mrproper
