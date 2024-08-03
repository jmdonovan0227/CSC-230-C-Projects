CC = gcc
CFLAGS = -Wall -std=c99 -g

# replace executable
replace: replace.o expand.o line.o

expand.o: expand.c expand.h

line.o: line.c line.h

replace.o: replace.c expand.h line.h

# clean command
clean:
	rm -f replace.o expand.o line.o
	rm -f replace
	rm -f output.txt stdout.txt stderr.txt