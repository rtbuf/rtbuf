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

install:
	${MAKE} -C ${PWD}/librtbuf install
	${MAKE} -C ${PWD}/lib      install
	${MAKE} -C ${PWD}/cli      install
	${MAKE} -C ${PWD}/gtk3     install

.PHONY: all build clean install
