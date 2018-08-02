
PROGRAM = rtbuf
SRCDIR = ../..

CPPFLAGS = -I${SRCDIR}/../cli/build/so
CFLAGS = -O0 -ggdb -W -Wall -Werror

LDFLAGS = -L${SRCDIR}/../cli/build/so
LIBS = -lcli -lpthread
HEADERS = symbol.h rtbuf_defs.h rtbuf_type.h rtbuf_fun.h rtbuf.h rtbuf_lib.h
OBJECTS = symbol.o rtbuf_type.o rtbuf_fun.o rtbuf.o rtbuf_lib.o \
	  rtbuf_cli.o

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
