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
CLEANFILES =

all: types libraries subdirs

clean: subdirs-clean
	rm -rf .libs *.o *.lo *.la ${CLEANFILES} *.gcno *.gcna *.gcda

install: libraries-install subdirs-install

.PHONY: all clean libraries subdirs subdirs-clean subdirs-install

include config.mk
