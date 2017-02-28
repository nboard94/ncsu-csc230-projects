CC = gcc
CFLAGS = -g -Wall -std=c99

# This is a common trick.  All is the first target, so it's the
# default.  We use it to build both of the executables we want.
all: jumble puzzle

jumble: jumble.o

puzzle: puzzle.o grid.o


# Another common trick, a clean rule to remove temporary files, or
# files we could easily rebuild.
clean:
	rm -f jumble jumble.o
	rm -f puzzle puzzle.o grid.o
	rm -f output.txt stderr.txt

