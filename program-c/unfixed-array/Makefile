all: c99 c89 gnu

c99: unfixed-array.c
	gcc -o c99 unfixed-array.c  -std=c99

c89: unfixed-array.c
	gcc -o c89 unfixed-array.c  -std=c89 -pedantic -pedantic-errors

gnu: unfixed-array.c
	gcc -o gnu unfixed-array.c  -std=gnu89

clean:
	rm c99 c89 gnu
