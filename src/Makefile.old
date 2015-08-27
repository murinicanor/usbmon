CCFLAGS = -O2 -g -Wall -Wextra -pedantic -std=c99
CPPFLAGS = -O2 -g -Wall -Wextra -pedantic -std=c++11
CC = gcc
CPP = g++

default: usbmon-dump usbmon-api


usbmon-dump: usbmon-dump.o
	$(CC) -o usbmon-dump usbmon-dump.o

usbmon-dump.o: usbmon-dump.c usbmon-dump.h
	$(CC) $(CFLAGS) -o usbmon-dump.o -c usbmon-dump.c


usbmon-api: usbmon-api.o usbmon.o usbpacket.o
	$(CPP) -o usbmon-api usbmon-api.o usbmon.o usbpacket.o -pthread

usbmon-api.o: usbmon-api.cpp usbmon.hpp
	$(CPP) $(CPPFLAGS) -c usbmon-api.cpp

usbmon.o: usbmon.cpp usbmon.hpp
	$(CPP) $(CPPFLAGS) -c usbmon.cpp

usbpacket.o: usbpacket.cpp usbpacket.hpp
	$(CPP) $(CPPFLAGS) -c usbpacket.cpp


clean:
	rm -f *.o

remove:
	rm usbmon-dump usbmon-api

