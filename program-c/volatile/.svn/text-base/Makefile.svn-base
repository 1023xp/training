all: volatile.asm no-volatile.asm

volatile.asm: volatile
	arm-linux-gnueabihf-objdump -d volatile > volatile.asm

no-volatile.asm: no-volatile
	arm-linux-gnueabihf-objdump -d no-volatile > no-volatile.asm
	
volatile:  volatile.c
	arm-linux-gnueabihf-gcc -o volatile volatile.c -O2 -g

no-volatile:  no-volatile.c
	arm-linux-gnueabihf-gcc -o no-volatile no-volatile.c -O2 -g

clean:
	rm volatile no-volatile volatile.asm no-volatile.asm
