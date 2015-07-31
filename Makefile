CFLAGS = -O2 -g -Wall -Wextra -pedantic
CC = gcc

default: usbmon-dump clean

usbmon-dump: usbmon-dump.o

clean:
	rm -f *.o
