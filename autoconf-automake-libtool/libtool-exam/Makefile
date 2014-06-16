OBJS = main.o fun.o hello.o
LO_OBJS = main.lo fun.lo hello.lo
PACKAGE_VERSION = 1:1:1
LIBDIR=/usr/local/lib

all : test

install : libhello.la test
	libtool --mode=install cp libhello.la /usr/local/lib/
	libtool -n --mode=finish /usr/local/lib
	libtool --mode=install cp test /usr/local/bin/
	libtool -n --mode=finish /usr/local/bin

test : libhello.la main.o
	libtool --mode=link gcc -g -O -o test main.o libhello.la -lm

libhello.la : $(OBJS)
	libtool --mode=link gcc -g -O -o libhello.la $(LO_OBJS) -rpath ${LIBDIR} -lm -version-info ${PACKAGE_VERSION}

%.o : %.c
	libtool --mode=compile gcc -g -O -c $<

clean :
	@rm -f OBJ/* lib*.a *~ *core *.lo *.o *.la
	@rm -rf .libs
