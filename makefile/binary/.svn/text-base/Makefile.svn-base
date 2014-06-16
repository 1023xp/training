test: main.o add.o
	gcc -o test main.o add.o

main.o: main.c add.h
	gcc -c main.c
add.o: add.c
	gcc -c add.c

clean:
	rm *.o
	rm test
