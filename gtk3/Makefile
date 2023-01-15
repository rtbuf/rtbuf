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

CLEANFILES = .libs *.o *.lo *.la rtbuf_gtk3 rtbuf_gtk3_asan rtbuf_gtk3_cov rtbuf_gtk3_debug *.gcno *.gcna *.gcda

build: rtbuf_gtk3

all:
	${MAKE} build
	if [ "x${HAVE_GCOV}" = "xYes" ]; then ${MAKE} cov; fi
	${MAKE} debug
	if [ "x${HAVE_ASAN}" = "xYes" ]; then ${MAKE} asan; fi

asan: rtbuf_gtk3_asan

clean:
	rm -rf ${CLEANFILES}

cov: rtbuf_gtk3_cov

debug: rtbuf_gtk3_debug

rtbuf_gtk3_UI = \
	rtbuf_gtk3_input_widget.ui \
	rtbuf_gtk3_modular.ui \
	rtbuf_gtk3_output_widget.ui \
	rtbuf_gtk3_widget.ui

rtbuf_gtk3_resources.c: rtbuf_gtk3.gresource.xml ${rtbuf_gtk3_UI}
	glib-compile-resources rtbuf_gtk3.gresource.xml --target=rtbuf_gtk3_resources.c --generate-source

CLEANFILES = rtbuf_gtk3_resources.c

.PHONY: all clean

include config.mk
include sources.mk
