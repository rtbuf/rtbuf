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
CLEANFILES=

all: rtbuf-gtk3

clean:
	rm -rf .libs *.o *.lo *.la rtbuf-gtk3 ${CLEANFILES} *.gcno *.gcna *.gcda

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
