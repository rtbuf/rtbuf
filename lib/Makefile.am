CLEANFILES =
rtbuf_includedir = $(includedir)/rtbuf
rtbuf_include_HEADERS =
rtbuf_libdir = $(libdir)/rtbuf
rtbuf_lib_LTLIBRARIES =
lib_LTLIBRARIES =

SUBDIRS = . signal sndio # dynamic glfw3 portaudio reverb sndio synth

# music

music_type: music_type.c include/rtbuf/music.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o music_type music_type.c
include/rtbuf/music_type.h: music_type
	./music_type > include/rtbuf/music_type.h
CLEANFILES += music_type include/rtbuf/music_type.h

rtbuf_lib_LTLIBRARIES += music.la
music_la_LIBADD = ${MUSIC_LIBS} ../librtbuf/librtbuf.la
music_la_LDFLAGS = -no-undefined -module

rtbuf_include_HEADERS += \
	include/rtbuf/music.h \
	include/rtbuf/music_type.h

# portaudio

if ENABLE_PORTAUDIO
portaudio_type: portaudio_type.c include/rtbuf/portaudio.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o portaudio_type portaudio_type.c
include/rtbuf/portaudio_type.h: portaudio_type
	./portaudio_type > include/rtbuf/portaudio_type.h
CLEANFILES += portaudio_type include/rtbuf/portaudio_type.h
portaudio.lo: include/rtbuf/portaudio_type.h
endif # ENABLE_PORTAUDIO


# reverb

reverb_type: reverb_type.c include/rtbuf/reverb.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o reverb_type reverb_type.c
include/rtbuf/reverb_type.h: reverb_type
	./reverb_type > include/rtbuf/reverb_type.h
CLEANFILES += reverb_type include/rtbuf/reverb_type.h
reverb.lo: include/rtbuf/reverb_type.h

# signal

signal_type: signal_type.c include/rtbuf/signal.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o signal_type signal_type.c
include/rtbuf/signal_type.h: signal_type
	./signal_type > include/rtbuf/signal_type.h
CLEANFILES += signal_type include/rtbuf/signal_type.h
signal.lo: include/rtbuf/signal_type.h

lib_LTLIBRARIES += librtbuf_signal.la
librtbuf_signal_la_LIBADD = ${SIGNAL_LIBS} ../librtbuf/librtbuf.la
librtbuf_signal_la_LDFLAGS = -no-undefined -module
librtbuf_signal_la_SOURCES = \
	signal.c

rtbuf_include_HEADERS += \
	include/rtbuf/signal.h \
	include/rtbuf/signal_type.h

# sndio

if ENABLE_SNDIO
sndio_type: sndio_type.c include/rtbuf/sndio.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o sndio_type sndio_type.c
include/rtbuf/sndio_type.h: sndio_type
	./sndio_type > include/rtbuf/sndio_type.h
CLEANFILES += sndio_type include/rtbuf/sndio_type.h
all-local: include/rtbuf/sndio_type.h
endif # ENABLE_SNDIO

# synth

synth_type: synth_type.c include/rtbuf/synth.h include/rtbuf/music_type.h include/rtbuf/signal_type.h
	${CC} ${CFLAGS} ${CPPFLAGS} -o synth_type synth_type.c
include/rtbuf/synth_type.h: synth_type
	./synth_type > include/rtbuf/synth_type.h
CLEANFILES += synth_type include/rtbuf/synth_type.h
synth.lo: include/rtbuf/synth_type.h
