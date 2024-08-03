CC = gcc
CFLAGS = -Wall -std=c99 -g

# Create our hash executable
hash: hash.o byteBuffer.o ripeMD.o
	gcc hash.o byteBuffer.o ripeMD.o -o hash

hash.o: hash.c byteBuffer.h ripeMD.h

byteBuffer.o: byteBuffer.c byteBuffer.h

ripeMD.o: ripeMD.c ripeMD.h byteBuffer.h

# Clean project rule, make sure to delete extra files later
clean:
	rm -f hash.o byteBuffer.o ripeMD.o
	rm -f hash testdriver
	rm -f output.txt stderr.txt

# testdriver executable command
testdriver:
	gcc -Wall -std=c99 -g -DTESTABLE testdriver.c ripeMD.c byteBuffer.c -o testdriver