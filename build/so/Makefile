
PROGRAM = cli.so
SRCDIR = ../..

CPPFLAGS =
CFLAGS = -O0 -ggdb -W -Wall -Werror -fpic

LDFLAGS = -fPIC -shared
LIBS = -lhistory -lreadline
HEADERS = cli.h
OBJECTS = cli.o

SRC = ${HEADERS} ${OBJECTS:%.o=%.c}

${PROGRAM}: ${OBJECTS}
	${CC} ${LDFLAGS} ${OBJECTS} ${LIBS} -o ${PROGRAM}

${OBJECTS}: ${HEADERS}

${SRC}: %: ${SRCDIR}/%
	cp $< $@

CLEANFILES += ${PROGRAM} ${OBJECTS} ${SRC}

clean:
	rm -f ${CLEANFILES}

.PHONY: clean
