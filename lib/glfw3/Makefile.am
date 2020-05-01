CLEANFILES =
rtbuf_includedir = $(includedir)/rtbuf/glfw3
rtbuf_include_HEADERS =
rtbuf_libdir = $(libdir)/rtbuf/glfw3
rtbuf_lib_LTLIBRARIES =

rtbuf_lib_LTLIBRARIES += keyboard.la
keyboard_la_CPPFLAGS = ${GLFW3_CFLAGS}
keyboard_la_LDFLAGS = -no-undefined -module -shared
keyboard_la_LIBADD = ${GLFW3_LIBS} ../librtbuf_music.la ../librtbuf_signal.la

rtbuf_lib_LTLIBRARIES += oscilloscope.la
oscilloscope_la_CPPFLAGS = ${GLFW3_CFLAGS}
oscilloscope_la_LDFLAGS = -no-undefined -module -shared
oscilloscope_la_LIBADD = ${GLFW3_LIBS} ../librtbuf_signal.la
