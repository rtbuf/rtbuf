if ENABLE_PORTAUDIO
CLEANFILES =
rtbuf_includedir = $(includedir)/rtbuf/portaudio
rtbuf_include_HEADERS =
rtbuf_libdir = $(libdir)/rtbuf/portaudio
rtbuf_lib_LTLIBRARIES =

rtbuf_lib_LTLIBRARIES += input.la
input_la_LDFLAGS = -no-undefined -module -shared
input_la_LIBADD = ${PORTAUDIO_LIBS} ../librtbuf_signal.la

rtbuf_lib_LTLIBRARIES += output.la
output_la_LDFLAGS = -no-undefined -module -shared
output_la_LIBADD = ${PORTAUDIO_LIBS} ../librtbuf_signal.la
endif
