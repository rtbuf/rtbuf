
ACLOCAL_AMFLAGS = -I build-aux/m4

bin_PROGRAMS =
CLEANFILES =
include_HEADERS =
lib_LTLIBRARIES =

if DEBUG
AM_CFLAGS = -DDEBUG -O0 -ggdb
else
AM_CFLAGS = -DNDEBUG -O2
endif

lib_LTLIBRARIES += librtbuf.la
librtbuf_la_CFLAGS = ${LIBRTBUF_CFLAGS}
librtbuf_la_LIBADD = ${LIBRTBUF_LIBS}
librtbuf_la_SOURCES = \
	symbol.c \
	rtbuf_type.c \
	rtbuf_proc.c \
	rtbuf_var.c \
	rtbuf.c
include_HEADERS += \
	symbol.h \
	rtbuf_defs.h \
	rtbuf_type.h \
	rtbuf_proc.h \
	rtbuf_var.h \
	rtbuf.h

if ENABLE_LIBDATA
librtbuf_la_SOURCES += \
	data.c
include_HEADERS += \
	data.h
endif

if ENABLE_GTK3
bin_PROGRAMS += rtbuf-gtk
rtbuf_gtk_CFLAGS = ${GTK3_CFLAGS}
rtbuf_gtk_LDADD = ${GTK3_LIBS} librtbuf.la
rtbuf_gtk_SOURCES = \
	rtbuf_gtk.c
endif # ENABLE_GTK3

if ENABLE_RTBUF
bin_PROGRAMS += rtbuf
rtbuf_LDADD = ${RTBUF_LIBS} librtbuf.la
rtbuf_SOURCES = \
	rtbuf_lib.c \
	rtbuf_cli.c
include_HEADERS += \
	rtbuf_lib.h

if ENABLE_CLI
rtbuf_SOURCES += \
	cli.c
include_HEADERS += \
	cli.h
endif #	ENABLE_CLI
endif # ENABLE_RTBUF

if ENABLE_MUSIC
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
endif # ENABLE_MUSIC

if ENABLE_SIGNAL
rtbuf_signal_type: rtbuf_signal_type.c rtbuf_signal.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o rtbuf_signal_type rtbuf_signal_type.c
rtbuf_signal_type.h: rtbuf_signal_type
	./rtbuf_signal_type > rtbuf_signal_type.h
CLEANFILES += rtbuf_signal_type rtbuf_signal_type.h
rtbuf_signal.lo: rtbuf_signal_type.h

lib_LTLIBRARIES += librtbuf_signal.la
librtbuf_signal_la_LIBADD = ${SIGNAL_LIBS} librtbuf_music.la librtbuf.la
librtbuf_signal_la_SOURCES = \
	rtbuf_signal.c \
	rtbuf_signal_delay.c \
	rtbuf_signal_flanger.c \
	rtbuf_signal_hipass.c \
	rtbuf_signal_hipass2.c \
	rtbuf_signal_lowpass.c \
	rtbuf_signal_lowpass2.c \
	rtbuf_signal_sinus.c \
	rtbuf_signal_square.c \
	rtbuf_signal_type.h
include_HEADERS += \
	rtbuf_signal.h \
	rtbuf_signal_type.h
endif # ENABLE_SIGNAL

lib_LTLIBRARIES += librtbuf_dynamic.la
librtbuf_dynamic_la_LIBADD = -lm librtbuf_signal.la librtbuf.la
librtbuf_dynamic_la_SOURCES = \
	rtbuf_dynamic.c \
	rtbuf_dynamic_limiter.c
include_HEADERS += \
	rtbuf_dynamic.h

rtbuf_synth_type: rtbuf_synth_type.c rtbuf_synth.h rtbuf_music_type.h rtbuf_signal_type.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o rtbuf_synth_type rtbuf_synth_type.c
rtbuf_synth_type.h: rtbuf_synth_type
	./rtbuf_synth_type > rtbuf_synth_type.h
CLEANFILES += rtbuf_synth_type rtbuf_synth_type.h
rtbuf_synth.lo: rtbuf_synth_type.h

lib_LTLIBRARIES += librtbuf_synth.la
librtbuf_synth_la_LIBADD = librtbuf_signal.la librtbuf_music.la librtbuf.la
librtbuf_synth_la_SOURCES = \
	rtbuf_music_type.h \
	rtbuf_signal_type.h \
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
rtbuf_sndio.lo: rtbuf_sndio_type.h

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
endif # ENABLE_SNDIO

if ENABLE_PORTAUDIO
rtbuf_portaudio_type: rtbuf_portaudio_type.c rtbuf_portaudio.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o rtbuf_portaudio_type rtbuf_portaudio_type.c
rtbuf_portaudio_type.h: rtbuf_portaudio_type
	./rtbuf_portaudio_type > rtbuf_portaudio_type.h
CLEANFILES += rtbuf_portaudio_type rtbuf_portaudio_type.h
rtbuf_portaudio.lo: rtbuf_portaudio_type.h

lib_LTLIBRARIES += librtbuf_portaudio.la
librtbuf_portaudio_la_LIBADD = ${PORTAUDIO_LIBS} librtbuf_signal.la librtbuf.la
librtbuf_portaudio_la_SOURCES = \
	rtbuf_portaudio.c \
	rtbuf_portaudio_input.c \
	rtbuf_portaudio_output.c \
	rtbuf_portaudio_type.h
include_HEADERS += \
	rtbuf_portaudio.h \
	rtbuf_portaudio_type.h
endif # ENABLE_PORTAUDIO

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
endif # ENABLE_GLFW3
