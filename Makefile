CFLAGS = -O2 -g -Wall -Wextra -pedantic
CC = gcc

default: usbmon clean

usbmon: usbmon.o

clean:
	rm -f *.o
