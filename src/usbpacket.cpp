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

#include "usbpacket.hpp"

#define DIRECTION_MASK 0x80

using namespace usbpacket;

UsbPacket::UsbPacket(){
	this->header = nullptr;
	this->data = nullptr;
}

int UsbPacket::parseUsbPacket(usbmon_get * get){
	
	if(get == nullptr || get->hdr == nullptr || get->data == nullptr){
		std::cerr << "UsbPacket::parseUsbPacket(usbmon_get * get) parameter is null\n";
		return EXIT_FAILURE;
	}

	this->data = (char *)get->data;
	this->header = get->hdr;	
	this->alloc = get->alloc;

	return EXIT_SUCCESS;
}

char * UsbPacket::getData(){
	return this->data;
}

size_t UsbPacket::getDataBufferSize(){
	return this->alloc;
}

uint32_t UsbPacket::getDataLength(){
	return this->header->len_cap;
}

unsigned char UsbPacket::getDeviceNumber(){
	return this->header->devnum;
}

uint16_t UsbPacket::getBusNumber(){
	return this->header->busnum;
}

Direction UsbPacket::getDirection(){
	return (this->header->epnum & DIRECTION_MASK) ? IN : OUT;
}

usbmon_packet * UsbPacket::getHeader(){
	return this->header;
}

void UsbPacket::printUsbPacket(){
	std::printf("%d.%03d %c %c ------%04x \n",
			this->getBusNumber(), this->getDeviceNumber(), this->header->type, 
			this->getDirection() == usbpacket::IN ? 'i' : 'o', this->getDataLength());
}

UsbPacket::~UsbPacket(){

}
