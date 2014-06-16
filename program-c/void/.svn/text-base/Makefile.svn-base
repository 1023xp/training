all: c99 c89 gnu

c99:  void.c
	gcc -o c99  void.c  -std=c99 ${CFLAGS}

c89:  void.c
	gcc -o c89  void.c  -std=c89 -pedantic -pedantic-errors ${CFLAGS}

gnu:  void.c
	gcc -o gnu  void.c -std=gnu89 ${CFLAGS}

clean:
	rm c99 c89 gnu
