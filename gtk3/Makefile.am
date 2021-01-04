if ENABLE_GTK3

bin_PROGRAMS = rtbuf-gtk3
rtbuf_gtk3_CFLAGS = ${GTK3_CFLAGS}
rtbuf_gtk3_LDADD = ${GTK3_LIBS} ../librtbuf/librtbuf.la \
	../cli/librtbuf_cli.la
rtbuf_gtk3_SOURCES = \
	list.c \
	queue.c \
	rtbuf_gtk3.c \
	rtbuf_gtk3_connection.c \
	rtbuf_gtk3_input.c \
	rtbuf_gtk3_library.c \
	rtbuf_gtk3_output.c \
	rtbuf_gtk3_resources.c \
	rtbuf_gtk3_rtbuf.c \
	rtbuf_gtk3_rtbuf_info.c \
	rtbuf_gtk3_input_widget.c \
	rtbuf_gtk3_output_widget.c \
	rtbuf_gtk3_widget.c \
	stack.c

rtbuf_includedir = $(includedir)/rtbuf
rtbuf_include_HEADERS = \
	list.h \
	queue.h \
	rtbuf_gtk3.h \
	rtbuf_gtk3_library.h \
	rtbuf_gtk3_rtbuf_info.h \
	rtbuf_gtk3_input_widget.h \
	rtbuf_gtk3_output_widget.h \
	rtbuf_gtk3_widget.h \
	stack.h

rtbuf_gtk3_UI = \
	rtbuf_gtk3_input_widget.ui \
	rtbuf_gtk3_modular.ui \
	rtbuf_gtk3_output_widget.ui \
	rtbuf_gtk3_widget.ui
rtbuf_gtk3_resources.c: rtbuf_gtk3.gresource.xml ${rtbuf_gtk3_UI}
	glib-compile-resources rtbuf_gtk3.gresource.xml --target=rtbuf_gtk3_resources.c --generate-source

CLEANFILES = \
	rtbuf_gtk3_resources.c

endif
