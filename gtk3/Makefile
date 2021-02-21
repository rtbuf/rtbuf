CLEANFILES=

all: rtbuf-gtk3

clean:
	rm -rf .libs *.o *.lo *.la rtbuf-gtk3 ${CLEANFILES}

rtbuf_gtk3_UI = \
	rtbuf_gtk3_input_widget.ui \
	rtbuf_gtk3_modular.ui \
	rtbuf_gtk3_output_widget.ui \
	rtbuf_gtk3_widget.ui

rtbuf_gtk3_resources.c: rtbuf_gtk3.gresource.xml ${rtbuf_gtk3_UI}
	glib-compile-resources rtbuf_gtk3.gresource.xml --target=rtbuf_gtk3_resources.c --generate-source

CLEANFILES = rtbuf_gtk3_resources.c

.PHONY: all clean

.include "config.mk"
