bin_PROGRAMS = rtbuf-gtk
rtbuf_gtk_CFLAGS = ${GTK3_CFLAGS}
rtbuf_gtk_LDADD = ${GTK3_LIBS} ../librtbuf/librtbuf.la
rtbuf_gtk_SOURCES = \
	rtbuf_gtk.c \
	rtbuf_gtk_connection.c \
	rtbuf_gtk_input.c \
	rtbuf_gtk_library.c \
	rtbuf_gtk_output.c \
	rtbuf_gtk_resources.c \
	rtbuf_gtk_rtbuf.c \
	rtbuf_input_widget.c \
	rtbuf_output_widget.c \
	rtbuf_widget.c

rtbuf_includedir = $(includedir)/rtbuf
rtbuf_include_HEADERS = \
	rtbuf_gtk.h \
	rtbuf_input_widget.h \
	rtbuf_output_widget.h \
	rtbuf_widget.h

rtbuf_gtk_UI = \
	rtbuf_input_widget.ui \
	rtbuf_modular.ui \
	rtbuf_output_widget.ui \
	rtbuf_widget.ui
rtbuf_gtk_resources.c: rtbuf_gtk.gresource.xml ${rtbuf_gtk_UI}
	glib-compile-resources rtbuf_gtk.gresource.xml --target=rtbuf_gtk_resources.c --generate-source

CLEANFILES = \
	rtbuf_gtk_resources.c
