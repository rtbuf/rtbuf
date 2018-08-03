
PROGRAM = rtbuf
SRCDIR = ../..

CPPFLAGS = -I${SRCDIR}/../cli/build/so
CFLAGS = -O0 -ggdb -W -Wall -Werror

LDFLAGS = -L../so -L${SRCDIR}/../cli/build/so
LIBS = -lrtbuf -lcli -lpthread
HEADERS = symbol.h rtbuf_defs.h rtbuf_type.h rtbuf_fun.h rtbuf.h rtbuf_lib.h
OBJECTS = rtbuf_lib.o rtbuf_cli.o

SRC = ${HEADERS} ${OBJECTS:%.o=%.c}

${PROGRAM}: ${OBJECTS} ../so/librtbuf.so
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

PREFIX ?= /usr/local

install:
	install -d -m 0755 "${PREFIX}/bin/"
	install -C -m 0755 ${PROGRAM} "${PREFIX}/bin/"
