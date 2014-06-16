all: likely-gnu.c likely-kr-i8.c likely-linux.c

likely-gnu.c: likely.c
	indent -gnu likely.c -o likely-gnu.c

likely-kr-i8.c: likely.c
	indent -gnu likely.c -o likely-kr-i8.c

likely-linux.c: likely.c
	indent -linux likely.c -o likely-linux.c

diff:
	vimdiff likely.c likely-gnu.c likely-kr-i8.c likely-linux.c

clean:
	rm likely-gnu.c likely-kr-i8.c likely-linux.c
