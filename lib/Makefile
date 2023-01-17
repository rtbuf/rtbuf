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

CLEANFILES = .libs *.o *.lo *.la *.gcno *.gcna *.gcda

build: types libraries subdirs

all: build

clean: subdirs-clean
	rm -rf ${CLEANFILES}

debug: types libraries-debug subdirs-debug

install: libraries-install subdirs-install

libraries: types

libraries-debug: types

.PHONY: all clean libraries subdirs subdirs-clean subdirs-install

subdirs: libraries

subdirs-debug: libraries-debug

include config.mk
