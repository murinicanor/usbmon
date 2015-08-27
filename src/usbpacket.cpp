#include "usbpacket.hpp"

#define DIRECTION_MASK 0x80

using namespace usbpacket;

UsbPacket::UsbPacket(){
	this->header = NULL;
	this->data = NULL;
}

int UsbPacket::parseUsbPacket(usbmon_get * get){
	
	if(get == NULL || get->hdr == NULL || get->data == NULL){
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