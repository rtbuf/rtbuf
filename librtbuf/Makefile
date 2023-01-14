## rtbuf
## Copyright 2018-2022 kmx.io <contact@kmx.io>
##
## Permission is hereby granted to use this software granted
## the above copyright notice and this permission paragraph
## are included in all copies and substantial portions of this
## software.
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
	if ${HAVE_GCOV}; then ${MAKE} cov; fi
	${MAKE} debug
	if ${HAVE_ASAN}; then ${MAKE} asan; fi

asan: librtbuf.asan.la librtbuf.asan.a

clean:
	rm -rf ${CLEANFILES}

clean_cov:
	rm -rf ${CLEANFILES_COV}

cov: librtbuf.cov.la librtbuf.cov.a

debug: librtbuf.debug.la librtbuf.debug.a

distclean:
	rm -rf ${DISTCLEANFILES}

gcovr:
	gcovr --gcov-executable ${GCOV} --html-details librtbuf.html

librtbuf.a: librtbuf.la
	cp .libs/librtbuf.a ./

librtbuf.asan.a: librtbuf.asan.la
	cp .libs/librtbuf.asan.a ./

librtbuf.cov.a: librtbuf.cov.la
	cp .libs/librtbuf.cov.a ./

librtbuf.debug.a: librtbuf.debug.la
	cp .libs/librtbuf.debug.a ./

include config.mk
include sources.mk
