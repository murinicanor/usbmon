#include "usbmon.hpp"
#include <iostream>

Usbmon::Usbmon () {
}

Usbmon::~Usbmon () {
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