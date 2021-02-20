all: build

build:
	${MAKE} -C librtbuf
	${MAKE} -C lib
	${MAKE} -C cli
	${MAKE} -C gtk3

clean:
	${MAKE} -C librtbuf clean
	${MAKE} -C lib      clean
	${MAKE} -C cli      clean
	${MAKE} -C gtk3     clean

install:
	${MAKE} -C librtbuf install
	${MAKE} -C lib      install
	${MAKE} -C cli      install
	${MAKE} -C gtk3     install

.PHONY: all build clean install
