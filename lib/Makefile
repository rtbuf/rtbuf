CLEANFILES =

all: types libraries subdirs

clean: subdirs-clean
	rm -rf .libs *.o *.lo *.la ${CLEANFILES}

install: subdirs-install

.PHONY: all clean libraries subdirs subdirs-clean subdirs-install

.include <config.mk>
