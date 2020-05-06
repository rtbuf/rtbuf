if ENABLE_SNDIO

CLEANFILES =
rtbuf_includedir = $(includedir)/rtbuf/sndio
rtbuf_include_HEADERS =
rtbuf_libdir = $(libdir)/rtbuf/sndio
rtbuf_lib_LTLIBRARIES =

rtbuf_lib_LTLIBRARIES += input.la
input_la_LDFLAGS = -no-undefined -module -shared
input_la_LIBADD = ${SNDIO_LIBS} ../librtbuf_signal.la

rtbuf_lib_LTLIBRARIES += output.la
output_la_LDFLAGS = -no-undefined -module -shared
output_la_LIBADD = ${SNDIO_LIBS} ../librtbuf_signal.la

endif
