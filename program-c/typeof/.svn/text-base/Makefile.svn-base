all: c99 c89 gnu

c99:  max.c
	gcc -o c99  max.c  -std=c99 ${CFLAGS}

c89:  max.c
	gcc -o c89  max.c  -std=c89 -pedantic -pedantic-errors ${CFLAGS}

gnu:  max.c
	gcc -o gnu  max.c -std=gnu89 -Werror ${CFLAGS}

clean:
	rm c99 c89 gnu
