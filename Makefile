
SUBDIRS = rtbuf rtbuf_signal rtbuf_sndio rtbuf_glfw3

all: ${SUBDIRS}
all: TARGET =

clean: ${SUBDIRS}
clean: TARGET = clean

rtbuf_signal_type: rtbuf_signal.h rtbuf_signal_type.c
	${CC} rtbuf_signal_type.c -o $@

rtbuf_signal_type.h: rtbuf_signal_type
	./rtbuf_signal_type > rtbuf_signal_type.h

rtbuf_sndio_type: rtbuf_sndio.h rtbuf_sndio_type.c
	${CC} rtbuf_sndio_type.c -o $@

rtbuf_sndio_type.h: rtbuf_sndio_type
	./rtbuf_sndio_type > rtbuf_sndio_type.h

${SUBDIRS}: %: rtbuf_signal_type.h rtbuf_sndio_type.h
	${MAKE} -C build/$@ ${TARGET}

.PHONY: all clean ${SUBDIRS}
