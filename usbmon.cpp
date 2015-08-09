#include "usbmon.hpp"
#include "usbpacket.hpp"
#include <iostream>
#include <memory>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>

#define MAXDATASIZE 10000


Usbmon::Usbmon () {
	this->usbmon_fd = -1;
	this->loopstate = false;
	this->rules = new std::list<std::shared_ptr<Rule>>();
}

int Usbmon::UsbmonInit(std::string usbmon_file_path){
	if((this->usbmon_fd = open(usbmon_file_path.c_str() , O_RDONLY)) < 0){
		std::cerr << "Cannot open file " + usbmon_file_path + "\n";
		return EXIT_FAILURE;
	}

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
	char * data;
	UsbPacket * packet;

	while(this->getLoopState()){

		memset(&arg, 0, sizeof(usbmon_get));
		memset(&pkt, 0, sizeof(usbmon_packet));

		packet = new UsbPacket();
		data = new char[MAXDATASIZE];

		arg.hdr = &pkt;
		arg.data = data;		
		arg.alloc = MAXDATASIZE;

		if(ioctl(this->getFileDescriptor(), MON_IOCX_GET, &arg) == -1){
			std::cerr << "ioctl failed\n";
			goto failure;
		}

		if(packet->parseUsbPacket(&arg)){
			std::cerr << "parseUsbPacket failed\n";
			goto failure;
		}

		packet->printUsbPacket();
		this->applyRules(packet);
		this->checkRules();
		

		delete [] data;
		delete packet;
		data = NULL;
		packet = NULL;
	}

	return EXIT_SUCCESS;

failure:
	delete [] data;
	delete packet;
	return EXIT_FAILURE;
}


void Usbmon::applyRules(UsbPacket * packet){
	
	if(packet == NULL)return; 

	this->mtx.lock();
	for (std::list<std::shared_ptr<Rule>>::iterator it=this->rules->begin(); it != this->rules->end(); it++){
		if(it->get()->getBusNumber() != packet->getBusNumber())continue;
		if(it->get()->getDeviceNumber() != packet->getDeviceNumber())continue;
		if(it->get()->getDirection() != BOTH && it->get()->getDirection() != packet->getDirection())continue;
		
		it->get()->addTransferedData(packet->getDataLength());
		std::cout << it->get()->getID() << " " << it->get()->getTransferedData() << std::endl;
	}
	this->mtx.unlock();
}

void Usbmon::checkRules(){
	this->mtx.lock();
	for (std::list<std::shared_ptr<Rule>>::iterator it=this->rules->begin(); it != this->rules->end(); it++){
		if(it->get()->getDataTransferLimit() >= 0 && it->get()->getTransferedData() > (uintmax_t)it->get()->getDataTransferLimit())
			std::cerr << it->get()->getID() << " busnum=" << (int)it->get()->getBusNumber() << " devnum=" << (int)it->get()->getDeviceNumber() << " limit=" << it->get()->getDataTransferLimit() << " transfered_data=" <<it->get()->getTransferedData() << std::endl;
	}
	this->mtx.unlock();
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
	this->usbmon_fd = fd;
	this->mtx.unlock();
}

int Usbmon::getFileDescriptor(){
	this->mtx.lock();
	int fd = this->usbmon_fd;
	this->mtx.unlock();
	return fd;
}


uint64_t Usbmon::addRule(uint16_t busnum, unsigned char devnum, Direction direction, intmax_t data_limit){
	this->mtx.lock();
	this->rules->push_back(std::shared_ptr<Rule>(new Rule(busnum, devnum, direction, data_limit)));
	uint64_t id = this->rules->back()->getID();
	this->mtx.unlock();
	return id;
}

int Usbmon::removeRule(uint64_t rule_id){
	std::shared_ptr<Rule> * rule = NULL;
	rule = this->getRule(rule_id);
	
	if(rule){
		this->mtx.lock();
		this->rules->remove(*rule);
		this->mtx.unlock();
	}	
	
	if(rule)return EXIT_SUCCESS;
	else return EXIT_FAILURE;
}

void Usbmon::clearRules(){
	this->mtx.lock();
	this->rules->clear();
	this->mtx.unlock();
}

int Usbmon::getNumOfRules(){
	this->mtx.lock();
	int count = this->rules->size();
	this->mtx.unlock();
	return count;
}


std::shared_ptr<Rule> * Usbmon::getRule(uint64_t rule_id){
	std::shared_ptr<Rule> * rule = NULL;
	this->mtx.lock();
	for (std::list<std::shared_ptr<Rule>>::iterator it=this->rules->begin(); it != this->rules->end(); it++){
		if(rule_id == it->get()->getID()){
			rule = &(*it);
			break;
		}
	}
	this->mtx.unlock();
	return rule;
}

/*****************************************************************************/

Rule::Rule(uint16_t busnum, unsigned char devnum, Direction direction, intmax_t data_limit){	
	this->devnum = devnum;
	this->busnum = busnum;
	this->direction = direction;
	this->data_limit = data_limit;
	this->transfered_data = 0;

	this->id = reinterpret_cast<uint64_t>(this);

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

uint64_t Rule::getID(){
	return this->id;
}

void Rule::addTransferedData(uintmax_t add){
	this->transfered_data += add;
}