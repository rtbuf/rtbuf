
CLEANFILES =
rtbuf_includedir = $(includedir)/rtbuf/portaudio
rtbuf_include_HEADERS =
rtbuf_libdir = $(libdir)/rtbuf/portaudio
rtbuf_lib_LTLIBRARIES =

rtbuf_lib_LTLIBRARIES += output.la
output_la_LDFLAGS = -no-undefined -module -shared
output_la_LIBADD = ${SIGNAL_LIBS} ../librtbuf_signal.la
