all: c99 c89 gnu

c99:  align.c
	gcc -o c99  align.c  -std=c99 ${CFLAGS}

c89:  align.c
	gcc -o c89  align.c  -std=c89 -pedantic -pedantic-errors ${CFLAGS}

gnu:  align.c
	gcc -o gnu  align.c -std=gnu89 ${CFLAGS}

clean:
	rm c99 c89 gnu
