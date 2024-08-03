CC = gcc
CFLAGS = -Wall -std=c99 -g

# Create kiosk executable
kiosk: kiosk.o input.o menu.o
	gcc kiosk.o input.o menu.o -o kiosk

kiosk.o: kiosk.c input.h menu.h

input.o: input.c input.h

menu.o: menu.c menu.h input.h

# Clean command delete object files, kiosk executable, and temporary files
clean:
	rm -f kiosk.o input.o menu.o
	rm -f kiosk
	rm -f output.txt stderr.txt stdout.txt