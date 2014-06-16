all: loader elf
loader: loader.c
	gcc -m32 -g -Wall -ldl -o loader loader.c
elf: elf.c
	gcc -m32 -pie -fPIE -o elf elf.c
