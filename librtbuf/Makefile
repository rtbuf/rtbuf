## rtbuf
## Copyright 2018-2023 kmx.io <contact@kmx.io>
##
## Permission is hereby granted to use this software excepted
## on Apple computers granted the above copyright notice and
## this permission paragraph are included in all copies and
## substantial portions of this software.
##
## THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
## PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
## AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
## THIS SOFTWARE.

CLEANFILES = *.a *.gcno *.la librtbuf.la .libs *.lo *.o

CLEANFILES_COV = *.css *.gcda *.html .libs/*.gcda
CLEANFILES += ${CLEANFILES_COV}

DISTCLEANFILES = ${CLEANFILES} config.mk

build: librtbuf.la librtbuf.a

all:
	${MAKE} build
	if [ "x${HAVE_GCOV}" = "xYes" ]; then ${MAKE} cov; fi
	${MAKE} debug
	if [ "x${HAVE_ASAN}" = "xYes" ]; then ${MAKE} asan; fi

asan: librtbuf_asan.la librtbuf_asan.a

clean:
	rm -rf ${CLEANFILES}

clean_cov:
	rm -rf ${CLEANFILES_COV}

cov: librtbuf_cov.la librtbuf_cov.a

debug: librtbuf_debug.la librtbuf_debug.a

distclean:
	rm -rf ${DISTCLEANFILES}

gcovr:
	gcovr --gcov-executable ${GCOV} --html-details librtbuf.html

librtbuf.a: librtbuf.la
	cp .libs/librtbuf.a ./

librtbuf_asan.a: librtbuf_asan.la
	cp .libs/librtbuf_asan.a ./

librtbuf_cov.a: librtbuf_cov.la
	cp .libs/librtbuf_cov.a ./

librtbuf_debug.a: librtbuf_debug.la
	cp .libs/librtbuf_debug.a ./

include config.mk
include sources.mk
