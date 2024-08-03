CC = gcc
CFLAGS = -Wall -std=c99 -g

# Create both our polish10 and polish32 executable
all: polish10 polish32

# Create our polish10 executable
polish10: polish.o base10.o bounds.o
	gcc polish.o base10.o bounds.o -o polish10

polish.o: polish.c base.h bounds.h

base10.o: base10.c base.h bounds.h

bounds.o: bounds.c bounds.h

# Create our polish32 executable
polish32: polish.o base32.o bounds.o
	gcc polish.o base32.o bounds.o -o polish32

polish.o: polish.c base.h bounds.h

base32.o: base32.c base.h bounds.h

bounds.o: bounds.c bounds.h

# Clean our project
clean:
	rm -f polish.o base10.o bounds.o base32.o
	rm -f polish10 polish32
	rm -f output-p10.txt output-p32.txt output.txt