all: symbol ref-s ref-m ref-w
symbol: var.o main.o
	gcc -o symbol var.o main.o
ref-s: single-multithrad.o
	gcc -o ref-s single-multithrad.o
ref-m: single-multithrad.o
	gcc -o ref-m  single-multithrad.o -lpthread
ref-w: test.o simple.o
	gcc -o ref-w test.o simple.o
clean:
	rm *.o symbol ref*

