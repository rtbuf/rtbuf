
PROGRAM = sndio.so
SRCDIR = ../..

CPPFLAGS =
CFLAGS = -O0 -ggdb -W -Wall -Werror -fpic

LDFLAGS = -fPIC -shared
LIBS = -lsndio
HEADERS = symbol.h rtbuf_defs.h rtbuf_type.h rtbuf_fun.h rtbuf.h \
	  rtbuf_lib.h rtbuf_music.h rtbuf_signal.h rtbuf_signal_type.h \
	  rtbuf_sndio.h rtbuf_sndio_type.h
OBJECTS = rtbuf_sndio.o rtbuf_sndio_input.o rtbuf_sndio_output.o

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
