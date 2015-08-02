#include "usbmon.hpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>

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

	while(this->getLoopState()){
		std::cout << "while cycle\n";
		this->setLoopState(false);
	}


	return EXIT_SUCCESS;
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