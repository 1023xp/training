all: likely.asm unlikely.asm

likely.asm: likely
	objdump -d likely > likely.asm

unlikely.asm: unlikely
	objdump -d unlikely > unlikely.asm
	
likely:  likely.c
	gcc -o likely likely.c -O2 -g

unlikely:  unlikely.c
	gcc -o unlikely unlikely.c -O2 -g

clean:
	rm likely unlikely likely.asm unlikely.asm
