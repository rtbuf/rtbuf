
ACLOCAL_AMFLAGS = -I build-aux/m4

bin_PROGRAMS =
CLEANFILES =
include_HEADERS =
lib_LTLIBRARIES =

AM_CPPFLAGS = ${local_cppflags}
AM_LDFLAGS = ${local_ldflags} -L./.libs

if DEBUG
AM_CFLAGS = -DDEBUG -O0 -ggdb
else
AM_CFLAGS = -DNDEBUG -O2
endif

lib_LTLIBRARIES += librtbuf.la
librtbuf_la_SOURCES = \
	symbol.c \
	rtbuf_type.c \
	rtbuf_fun.c \
	rtbuf.c
include_HEADERS += \
	symbol.h \
	rtbuf_defs.h \
	rtbuf_type.h \
	rtbuf_fun.h \
	rtbuf.h

if ENABLE_RTBUF
bin_PROGRAMS += rtbuf
rtbuf_LDADD = ${RTBUF_LIBS} librtbuf.la
rtbuf_SOURCES = \
	rtbuf_lib.c \
	rtbuf_cli.c
include_HEADERS += \
	rtbuf_lib.h
endif

rtbuf_music_type: rtbuf_music_type.c rtbuf_music.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o rtbuf_music_type rtbuf_music_type.c
rtbuf_music_type.h: rtbuf_music_type
	./rtbuf_music_type > rtbuf_music_type.h
CLEANFILES += rtbuf_music_type rtbuf_music_type.h

lib_LTLIBRARIES += librtbuf_music.la
librtbuf_music_la_LIBADD = ${MUSIC_LIBS} librtbuf.la
librtbuf_music_la_SOURCES = \
	rtbuf_music.c
include_HEADERS += \
	rtbuf_music.h \
	rtbuf_music_type.h

if ENABLE_SIGNAL
rtbuf_signal_type: rtbuf_signal_type.c rtbuf_signal.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o rtbuf_signal_type rtbuf_signal_type.c
rtbuf_signal_type.h: rtbuf_signal_type
	./rtbuf_signal_type > rtbuf_signal_type.h
CLEANFILES += rtbuf_signal_type rtbuf_signal_type.h

lib_LTLIBRARIES += librtbuf_signal.la
librtbuf_signal_la_LIBADD = ${SIGNAL_LIBS} librtbuf_music.la librtbuf.la
librtbuf_signal_la_SOURCES = \
	rtbuf_signal.c \
	rtbuf_signal_sinus.c \
	rtbuf_signal_square.c \
	rtbuf_signal_type.h
include_HEADERS += \
	rtbuf_signal.h \
	rtbuf_signal_type.h
endif

lib_LTLIBRARIES += librtbuf_dynamic.la
librtbuf_dynamic_la_LIBADD = -lm librtbuf_signal.la librtbuf.la
librtbuf_dynamic_la_SOURCES = \
	rtbuf_dynamic.c \
	rtbuf_dynamic_limiter.c
include_HEADERS += \
	rtbuf_dynamic.h

rtbuf_synth_type: rtbuf_synth_type.c rtbuf_synth.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o rtbuf_synth_type rtbuf_synth_type.c
rtbuf_synth_type.h: rtbuf_synth_type
	./rtbuf_synth_type > rtbuf_synth_type.h
CLEANFILES += rtbuf_synth_type rtbuf_synth_type.h

lib_LTLIBRARIES += librtbuf_synth.la
librtbuf_synth_la_LIBADD = librtbuf_signal.la librtbuf_music.la librtbuf.la
librtbuf_synth_la_SOURCES = \
	rtbuf_synth.c \
	rtbuf_synth_adsr.c \
	rtbuf_synth_synth.c
include_HEADERS += \
	rtbuf_synth.h \
	rtbuf_synth_type.h

if ENABLE_SNDIO
rtbuf_sndio_type: rtbuf_sndio_type.c rtbuf_sndio.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o rtbuf_sndio_type rtbuf_sndio_type.c
rtbuf_sndio_type.h: rtbuf_sndio_type
	./rtbuf_sndio_type > rtbuf_sndio_type.h
CLEANFILES += rtbuf_sndio_type rtbuf_sndio_type.h

lib_LTLIBRARIES += librtbuf_sndio.la
librtbuf_sndio_la_LIBADD = ${SNDIO_LIBS} librtbuf_signal.la librtbuf.la
librtbuf_sndio_la_SOURCES = \
	rtbuf_sndio.c \
	rtbuf_sndio_input.c \
	rtbuf_sndio_output.c \
	rtbuf_sndio_type.h
include_HEADERS += \
	rtbuf_sndio.h \
	rtbuf_sndio_type.h
endif

if ENABLE_GLFW3
lib_LTLIBRARIES += librtbuf_glfw3.la
librtbuf_glfw3_la_CPPFLAGS = ${GLFW3_CFLAGS}
librtbuf_glfw3_la_LIBADD = ${GLFW3_LIBS} librtbuf_music.la librtbuf_signal.la librtbuf.la
librtbuf_glfw3_la_SOURCES = \
	rtbuf_glfw3.c \
	rtbuf_glfw3_keyboard.c \
	rtbuf_glfw3_oscilloscope.c
include_HEADERS += \
	rtbuf_glfw3.h
endif

deps_headers = ${include_HEADERS}
deps_sources = \
	${librtbuf_la_SOURCES} \
	${rtbuf_SOURCES} \
	${rtbuf_signal_type_SOURCES} \
	${librtbuf_signal_la_SOURCES} \
	${rtbuf_sndio_type_SOURCES} \
	${librtbuf_sndio_la_SOURCES} \
	${librtbuf_glfw3_la_SOURCES}

dependencies.mk: ${deps_headers} ${deps_sources}
	${CC} ${CPPFLAGS} ${CFLAGS} -M *.c > dependencies.mk.1
	sed -e 's/^\(.*\)[.]o:/\1.$$(OBJEXT) \1.lo:/' dependencies.mk.1 > dependencies.mk.2
	rm dependencies.mk.1
	mv dependencies.mk.2 dependencies.mk
CLEANFILES += dependencies.mk.1 dependencies.mk.2
