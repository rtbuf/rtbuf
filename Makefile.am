
SUBDIRS = librtbuf cli gtk lib

if ENABLE_NSIS
rtbuf-${PACKAGE_VERSION}_setup.exe: .libs/rtbuf.exe .libs/rtbuf-gtk.exe
	makensis rtbuf.nsi
all: rtbuf-${PACKAGE_VERSION}_setup.exe
endif # ENABLE_NSIS
