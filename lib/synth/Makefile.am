CLEANFILES =
rtbuf_includedir = $(includedir)/rtbuf/synth
rtbuf_include_HEADERS =
rtbuf_libdir = $(libdir)/rtbuf/synth
rtbuf_lib_LTLIBRARIES =

rtbuf_lib_LTLIBRARIES += adsr.la
adsr_la_LDFLAGS = -no-undefined -module -shared
adsr_la_LIBADD = ../librtbuf_signal.la

rtbuf_lib_LTLIBRARIES += synth.la
synth_la_LDFLAGS = -no-undefined -module -shared
synth_la_LIBADD = ../librtbuf_music.la ../librtbuf_signal.la
