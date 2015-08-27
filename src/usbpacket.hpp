// --usbmonitor--
//
// Copyright (C) 2015  Radovan Sroka
//
// This file is part of usbmonitor.
//
// UsbMonitor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef USBPACKET_HPP
#define USBPACKET_HPP

#include <iostream>
#include <cstdio>
#include <cinttypes>
#include <cstdlib>
#include <cstring>

#define BUFFERSIZE 4096

namespace usbpacket{

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

struct mon_bin_stats {
	uint32_t queued;
	uint32_t dropped;
};

enum Direction{
	IN, OUT, BOTH
};

class UsbPacket{

public:
	UsbPacket();

	int parseUsbPacket(usbmon_get * get);

	char * getData();
	size_t getDataBufferSize();
	uint32_t getDataLength();
	unsigned char getDeviceNumber();
	uint16_t getBusNumber();
	Direction getDirection();
	usbmon_packet * getHeader();

	void printUsbPacket();

	~UsbPacket();

private:
	usbmon_packet * header;
	char * data;
	size_t alloc;
	
};

}

#endif
