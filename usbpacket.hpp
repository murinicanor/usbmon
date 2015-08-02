#ifndef USBPACKET_HPP
#define USBPACKET_HPP

#include <iostream>
#include <cstdio>
#include <cinttypes>
#include <cstdlib>


struct usbmon_packet {
	uint64_t id;			/* URB ID */
	unsigned char type;	/* 'S'ubmit 'C'allback 'E'rror */
	unsigned char xfer;	/* ISO=0 INT=1 CTRL=2 BULK=3 */
	unsigned char epnum;
	unsigned char devnum;
	uint16_t busnum;
	char flag_setup;
	char flag_data;
	int64_t ts_sec;
	int32_t ts_usec;
	int32_t status;
	uint32_t length;
	uint32_t len_cap;
	union {
		unsigned char setup[8];
		struct iso_rec {
			int32_t error_count;
			int32_t numdesc;
		} iso;
	} s;
	int32_t interval;
	int32_t start_frame;
	uint32_t xfer_flags;
	uint32_t ndesc;
};

struct usbmon_get {
	struct usbmon_packet *hdr;
	void *data;
	size_t alloc;
};


enum Direction{
	IN, OUT
};

class UsbPacket{

public:
	UsbPacket(usbmon_get * get);

	char * getData();
	size_t getDataBufferSize();
	uint32_t getDataLength();
	unsigned char getDeviceNumber();
	uint16_t getBusNumber();
	Direction getDirection();
	usbmon_packet * getHeader();

	~UsbPacket();

private:
	usbmon_packet * header;
	char * data;
	size_t alloc;
	
};


#endif