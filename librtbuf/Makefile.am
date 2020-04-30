
lib_LTLIBRARIES = librtbuf.la
librtbuf_la_CFLAGS = ${LIBRTBUF_CFLAGS}
librtbuf_la_LDFLAGS = -no-undefined
librtbuf_la_LIBADD = ${LIBRTBUF_LIBS}
librtbuf_la_SOURCES = \
	data.c \
	symbol.c \
	rtbuf_lib.c \
	rtbuf_type.c \
	rtbuf_proc.c \
	rtbuf_var.c \
	rtbuf.c

rtbuf_includedir = $(includedir)/rtbuf
rtbuf_include_HEADERS = \
	include/rtbuf/data.h \
	include/rtbuf/symbol.h \
	include/rtbuf/defs.h \
	include/rtbuf/lib.h \
	include/rtbuf/proc.h \
	include/rtbuf/type.h \
	include/rtbuf/var.h \
	include/rtbuf/rtbuf.h
