#
# © Copyright IBM Corp. 2008.  All Rights Reserved.
#

VER_MAJOR=0
VER_MINOR=7.6

DIST_TARGET=rtcheck-$(VER_MAJOR).$(VER_MINOR).tar.bz2
DIST_FILES=Makefile rtcheck.c rtcheck.init README CHANGELOG
DIRNAME=rtcheck

CC = gcc
CFLAGS =  -g -O2 -W -Wall
LIBS = -ldl -lpthread -lrt

build: all

.PHONY: all clean install

all: rtcheck dist

rtcheck: rtcheck.c
	$(CC) -o $@ $< $(CFLAGS) -D_XOPEN_SOURCE=600 -D__USE_UNIX98=1 $(LIBS) -DVERSION="\"$(VER_MAJOR).$(VER_MINOR)\""


dist: $(DIST_FILES)
	tar -C .. -cjf $(DIST_TARGET) \
		$(DIRNAME)/Makefile \
		$(DIRNAME)/rtcheck.c \
		$(DIRNAME)/rtcheck.init \
		$(DIRNAME)/CHANGELOG \
		$(DIRNAME)/README


install: $(DIST_FILES)
	install -m 0755 rtcheck /usr/bin
	install -m 0644 README /usr/share/rtcheck-$(VER_MAJOR).$(VER_MINOR)
	install -m 755 rtcheck.init /etc/init.d/rtcheck
	chkconfig rtcheck on

clean:
	rm -f rtcheck

distclean: clean
	rm -f $(DIST_TARGET)
