#include "usbmon.hpp"
#include "usbpacket.hpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>

#define MAXDATASIZE 10000


Usbmon::Usbmon () {
	this->usbmon_fd = -1;
	this->loopstate = false;
}

int Usbmon::UsbmonInit(std::string usbmon_file_path){
	if((this->usbmon_fd = open(usbmon_file_path.c_str() , O_RDONLY)) < 0){
		std::cerr << "Cannot open file " + usbmon_file_path + "\n";
		return EXIT_FAILURE;
	}

	this->rules = new std::list<std::shared_ptr<Rule>>();

	return EXIT_SUCCESS;
}

Usbmon::~Usbmon () {
	if(this->usbmon_fd > 0)close(this->usbmon_fd);
	delete rules;
}

int Usbmon::monitorLoop(){

	this->setLoopState(true);
	usbmon_get arg;
	usbmon_packet pkt;
	char * data = new char[MAXDATASIZE];
	arg.hdr = &pkt;
	arg.data = data;

	UsbPacket *packet = new UsbPacket();

	while(this->getLoopState()){


		arg.alloc = sizeof(data);
		if(ioctl(this->getFileDescriptor(), MON_IOCX_GET, &arg) < 0){
			std::cerr << "ioctl failed\n";
			goto failure;
		}

		if(packet->parseUsbPacket(&arg)){
			std::cerr << "parseUsbPacket failed\n";
			goto failure;
		}

		packet->printUsbPacket();

		//std::cout << uintmax_t((uint32_t(0) - 1) * 2);
		//this->setLoopState(false);
	}

	delete data;
	delete packet;
	return EXIT_SUCCESS;

failure:
	delete data;
	delete packet;
	return EXIT_FAILURE;
}

void Usbmon::setLoopState(bool state){
	this->mtx.lock();
	this->loopstate = state;
	this->mtx.unlock();
}

bool Usbmon::getLoopState(){
	this->mtx.lock();
	bool state = this->loopstate;
	this->mtx.unlock();
	return state;
}

void Usbmon::setFileDescriptor(int fd){
	this->mtx.lock();
	this->usbmon_fd;
	this->mtx.unlock();
}

int Usbmon::getFileDescriptor(){
	this->mtx.lock();
	int fd = this->usbmon_fd;
	this->mtx.unlock();
	return fd;
}

Rule::Rule(unsigned char devnum, uint16_t busnum,	Direction direction, intmax_t data_limit){
	this->devnum = devnum;
	this->busnum = busnum;
	this->direction = direction;
	this->data_limit = data_limit;
	this->transfered_data = 0;
}

Rule::~Rule(){

}

unsigned char Rule::getDeviceNumber(){
	return this->devnum;
}

uint16_t Rule::getBusNumber(){
	return this->busnum;
}

Direction Rule::getDirection(){
	return this->direction;
}

uintmax_t Rule::getTransferedData(){
	return this->transfered_data * 2;
}

intmax_t Rule::getDataTransferLimit(){
	return this->data_limit;
}

void Rule::setDeviceNumber(unsigned char num){
	this->devnum = num;
}

void Rule::setBusNumber(uint16_t num){
	this->busnum = num;
}

void Rule::setDirection(Direction direction){
	this->direction = direction;
}

void Rule::setDataTransferLimit(intmax_t limit){
	this->data_limit = limit;
}