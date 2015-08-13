#include "usbpacket.hpp"

using namespace usbpacket;

UsbPacket::UsbPacket(){
	this->header = new usbmon_packet;
	this->data = NULL;
}

int UsbPacket::parseUsbPacket(usbmon_get * get){
	
	if(get == NULL || get->hdr == NULL || get->data == NULL){
		std::cerr << "UsbPacket::parseUsbPacket(usbmon_get * get) parameter is null\n";
		return EXIT_FAILURE;
	}

	if (get->hdr->flag_data != 60){
		this->data = new char[get->hdr->len_cap * 2];
		memcpy(this->data, get->data, get->hdr->len_cap * 2);
	}


	memcpy(this->header, get->hdr, sizeof(usbmon_packet));
	
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

void UsbPacket::printUsbPacket(){
	std::printf("%d.%03d.%03d %c %c ------%04x \n",
			this->getBusNumber(), this->getDeviceNumber(), this->header->epnum & 0x7F,
			this->header->type, (this->header->epnum & 0x80) ? 'i' : 'o',this->header->length);
}

UsbPacket::~UsbPacket(){
	delete this->header;
	delete [] this->data;
}