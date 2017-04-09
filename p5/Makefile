CC = gcc
CFLAGS = -g -Wall -std=c99
LDLIBS = -lm

main: encrypt decrypt

encrypt: encrypt.o bits.o codes.o 

encrypt.o: encrypt.c bits.h codes.h

decrypt: decrypt.o bits.o codes.o

decrypt.o: decrypt.c bits.h codes.h

bits.o: bits.c bits.h codes.h

codes.o: codes.c bits.h codes.h

clean:
	rm -f encrypt encrypt.o
	rm -f decrypt decrypt.o
	rm -f bits.o codes.o
	rm -f output.txt stderr.txt
   

