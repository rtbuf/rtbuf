lib_LTLIBRARIES = librtbuf_cli.la
librtbuf_cli_la_LIBADD = ../librtbuf/.libs/librtbuf.la ${RTBUF_LIBS}
librtbuf_cli_la_SOURCES = \
	cli.c \
	rtbuf_cli.c

bin_PROGRAMS = rtbuf
rtbuf_LDADD = librtbuf_cli.la
rtbuf_SOURCES = \
	rtbuf_cli_main.c

rtbuf_includedir = $(includedir)/rtbuf
rtbuf_include_HEADERS = \
	include/rtbuf/cli.h
