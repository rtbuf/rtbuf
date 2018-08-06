
ACLOCAL_AMFLAGS = -I build-aux/m4

bin_PROGRAMS =
CLEANFILES =
include_HEADERS =
lib_LTLIBRARIES =

AM_CPPFLAGS = ${local_cppflags}
AM_LDFLAGS = ${local_ldflags}

if DEBUG
AM_CFLAGS = -O0 -ggdb -W -Wall -Werror
else
AM_CFLAGS = -O2 -DNDEBUG -W -Wall -Werror
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

if ENABLE_SIGNAL
rtbuf_signal_type: rtbuf_signal_type.c rtbuf_signal.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o rtbuf_signal_type rtbuf_signal_type.c
rtbuf_signal_type.h: rtbuf_signal_type
	./rtbuf_signal_type > rtbuf_signal_type.h
CLEANFILES += rtbuf_signal_type rtbuf_signal_type.h

lib_LTLIBRARIES += librtbuf_signal.la
librtbuf_signal_la_LIBADD = ${SIGNAL_LIBS}
librtbuf_signal_la_SOURCES = \
	rtbuf_music.c \
	rtbuf_signal.c \
	rtbuf_signal_sinus.c \
	rtbuf_signal_square.c \
	rtbuf_signal_adsr.c \
	rtbuf_signal_synth.c \
	rtbuf_signal_type.h
include_HEADERS += \
	rtbuf_music.h \
	rtbuf_signal.h \
	rtbuf_signal_type.h
endif

if ENABLE_SNDIO
rtbuf_sndio_type: rtbuf_sndio_type.c rtbuf_sndio.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o rtbuf_sndio_type rtbuf_sndio_type.c
rtbuf_sndio_type.h: rtbuf_sndio_type
	./rtbuf_sndio_type > rtbuf_sndio_type.h
CLEANFILES += rtbuf_sndio_type rtbuf_sndio_type.h

lib_LTLIBRARIES += librtbuf_sndio.la
librtbuf_sndio_la_LIBADD = ${SNDIO_LIBS}
librtbuf_sndio_la_SOURCES = \
	rtbuf_sndio.c \
	rtbuf_sndio_input.c \
	rtbuf_sndio_output.c \
	rtbuf_sndio_type.h
include_HEADERS += \
	rtbuf_sndio.h \
	rtbuf_sndio_type.h
endif

if ENABLE_GLFW
lib_LTLIBRARIES += librtbuf_glfw3.la
librtbuf_glfw3_la_CPPFLAGS = ${X11_CPPFLAGS}
librtbuf_glfw3_la_LDFLAGS = ${X11_LDFLAGS}
librtbuf_glfw3_la_LIBADD = ${GLFW_LIBS}
librtbuf_glfw3_la_SOURCES = \
	rtbuf_glfw3.c \
	rtbuf_music.c
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
	${CC} ${CPPFLAGS} ${CFLAGS} -M ${deps_sources} > dependencies.mk.1
	sed -e 's/^\(.*\)[.]o:/\1.$$(OBJEXT) \1.lo:/' dependencies.mk.1 > dependencies.mk.2
	rm dependencies.mk.1
	mv dependencies.mk.2 dependencies.mk
CLEANFILES += dependencies.mk.1 dependencies.mk.2
