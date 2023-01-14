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

all: build

build:
	${MAKE} -C ${PWD}/librtbuf
	${MAKE} -C ${PWD}/lib
	${MAKE} -C ${PWD}/cli
	${MAKE} -C ${PWD}/gtk3

clean:
	${MAKE} -C ${PWD}/librtbuf clean
	${MAKE} -C ${PWD}/lib      clean
	${MAKE} -C ${PWD}/cli      clean
	${MAKE} -C ${PWD}/gtk3     clean

doc:
	doxygen doxygen.conf

install:
	${MAKE} -C ${PWD}/librtbuf install
	${MAKE} -C ${PWD}/lib      install
	${MAKE} -C ${PWD}/cli      install
	${MAKE} -C ${PWD}/gtk3     install

licence:
	update_header config.subr ${RTBUF_CONFIGURES}
	update_header Makefile ${RTBUF_MAKEFILES}
	update_header librtbuf/rtbuf.h ${RTBUF_C_SOURCES}

.PHONY: all build clean doc install

include config.mk
