#include <iostream>
#include "usbmon.hpp"

using namespace std;

void callback_function(usbmonitor::CallBackMessage msg, std::shared_ptr<usbmonitor::Rule> sh_ptr_rule){
	if(msg == usbmonitor::BROKEN_RULE)
		std::cerr << sh_ptr_rule->getID() << " busnum=" << (int)sh_ptr_rule->getBusNumber() << " devnum=" << (int)sh_ptr_rule->getDeviceNumber() << " limit=" << sh_ptr_rule->getDataTransferLimit() << " transfered_data=" <<sh_ptr_rule->getTransferedData() << std::endl;
}


int main(int argc, char const *argv[]){

	usbmonitor::Usbmon * usbmon;
	usbmon = new usbmonitor::Usbmon(callback_function);
	usbmon->UsbmonInit("/dev/usbmon0");
	usbmon->monitorLoop();

	uint64_t first = usbmon->addRule(1,10, usbpacket::BOTH, 100);
	//uint64_t second = usbmon->addRule(1,9, usbpacket::IN, 1000);

	std::cout << usbmon->getNumOfRules() << std::endl;

	//usbmon->removeRule(first);
	//usbmon->removeRule(second);

	std::cout << usbmon->getNumOfRules() << std::endl;

	char c = ' ';
	while(c != 'q'){
		std::cin >> c;
		usbmon->setLoopState(false);
	}

	usbmon->waitThread();

	delete usbmon;

	return 0;
}