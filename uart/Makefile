all: main

clean:
	rm main.asm main.cod main.hex main.lst main.o

main: $(OBJ)
	sdcc --use-non-free -mpic14 -p16f887 main.c

upload:
	pk2cmd -P -Fmain.hex -M
