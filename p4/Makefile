CC = gcc
CFLAGS = -D_GNU_SOURCE -g -Wall -std=c99
LDLIBS = -lm

drawing: drawing.o scene.o model.o

drawing.o: drawing.c scene.h model.h

scene.o: scene.c scene.h model.h

model.o: model.c model.h

clean:
	rm -f drawing drawing.o
	rm -f scene.o model.o
	rm -f output.txt stderr.txt
  