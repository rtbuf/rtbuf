
PROGRAM = glfw3.so
SRCDIR = ../..

CPPFLAGS = -I/usr/local/include -I/usr/X11R6/include
CFLAGS = -O0 -ggdb -W -Wall -Werror -fpic

LDFLAGS = -fPIC -shared
LIBS = -lsndio
HEADERS = symbol.h rtbuf_defs.h rtbuf_type.h rtbuf_fun.h rtbuf.h \
	  rtbuf_lib.h rtbuf_signal.h rtbuf_music.h rtbuf_glfw3.h
OBJECTS = rtbuf_glfw3.o rtbuf_music.o

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

PREFIX ?= /usr/local

install:
	install -d -m 0755 "${PREFIX}/lib/rtbuf/"
	install -C -m 0755 ${PROGRAM} "${PREFIX}/lib/rtbuf/"
