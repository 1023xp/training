main: libadddec.a main.c
	gcc main.c -L. -ladddec -I. -o main
libadddec.a: add.o dec.o
	ar rs libadddec.a add.o dec.o
clean:
	rm *.o
	rm *.a
	rm main
