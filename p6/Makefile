CC = gcc
CFLAGS = -std=c99 -g

# Construct all files
interpret: interpret.o parse.o syntax.o value.o

interpret.o: interpret.c parse.h syntax.h value.h

parse.o: parse.c syntax.h value.h

syntax.o: syntax.c value.h

value.o: value.c

# Clean program
clean:
	rm -f interpret.o parse.o syntax.o value.o
	rm -f interpret
	rm -f output.txt stderr.txt stdout.txt