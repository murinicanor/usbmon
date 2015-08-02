#include "usbpacket.hpp"

UsbPacket::UsbPacket(){

}

int UsbPacket::parseUsbPacket(usbmon_get * get){
	
	if(get == NULL || get->hdr == NULL || get->data == NULL){
		std::cerr << "UsbPacket::parseUsbPacket(usbmon_get * get) parameter is null\n";
		return EXIT_FAILURE;
	}

	this->header = get->hdr;
	this->data = (char *)get->data;
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
	return (this->header->epnum & 0x80) ? IN : OUT;
}

usbmon_packet * UsbPacket::getHeader(){
	return this->header;
}

UsbPacket::~UsbPacket(){
	free(this->header);
	free(this->data);
}