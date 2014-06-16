all: c89 gnu

c89:  zero-array.c
	gcc -o c89  zero-array.c  -std=c89 -pedantic -pedantic-errors ${CFLAGS}

gnu:  zero-array.c
	gcc -o gnu  zero-array.c -std=gnu89 ${CFLAGS}

clean:
	rm c89 gnu
