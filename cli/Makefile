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

build: librtbuf_cli.la librtbuf_cli.a rtbuf

all:
	${MAKE} build
	if [ "x${HAVE_GCOV}" = "xYes" ]; then ${MAKE} cov; fi
	${MAKE} debug
	if [ "x${HAVE_ASAN}" = "xYes" ]; then ${MAKE} asan; fi

asan: rtbuf_asan

clean:
	rm -rf .libs *.o *.lo *.la rtbuf *.gcno *.gcna *.gcda

cov: rtbuf_cov

debug: rtbuf_debug

librtbuf_cli.a: librtbuf_cli.la
	cp .libs/librtbuf_cli.a ./

librtbuf_cli_asan.a: librtbuf_cli_asan.la
	cp .libs/librtbuf_cli_asan.a ./

librtbuf_cli_cov.a: librtbuf_cli_cov.la
	cp .libs/librtbuf_cli_cov.a ./

librtbuf_cli_debug.a: librtbuf_cli_debug.la
	cp .libs/librtbuf_cli_debug.a ./

.PHONY: all clean

include config.mk
include sources.mk
