
PROGRAM = demo_ord
SRCDIR = ../..

CPPFLAGS = -I${SRCDIR}/../cli/build/so
CFLAGS = -O0 -ggdb -W -Wall -Werror

LDFLAGS = -L${SRCDIR}/../cli/build/so
LIBS = -lcli
HEADERS = ord.h
OBJECTS = ord.o demo_ord.o

SRC = ${HEADERS} ${OBJECTS:%.o=%.c}

${PROGRAM}: ${OBJECTS}
	${CC} ${LDFLAGS} ${OBJECTS} ${LIBS} -o ${PROGRAM}

${OBJECTS}: ${HEADERS}

${SRC}: %: ${SRCDIR}/%
	if [ -f $@ ]; then chmod u+w $@; fi
	cp $< $@
	chmod a-w $@

CLEANFILES += ${PROGRAM} ${OBJECTS} ${SRC}

clean:
	rm -f ${CLEANFILES}

.PHONY: clean
