CFLAGS = -O2 -g -Wall -Wextra -pedantic
CC = gcc


usbmon: usbmon.o
	
clean:
	rm -f *.o
