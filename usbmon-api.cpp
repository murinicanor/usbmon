#include <iostream>
#include <thread>
#include <unistd.h>

#include "usbpacket.hpp"
#include "usbmon.hpp"

using namespace std;


int main(int argc, char const *argv[])
{
	usbmonitor::Usbmon * usbmon;
	usbmon = new usbmonitor::Usbmon();
	usbmon->UsbmonInit("/dev/usbmon0");
	usbmon->monitorLoop();

	uint64_t first = usbmon->addRule(1,8, usbpacket::BOTH, 500);
	uint64_t second = usbmon->addRule(1,9, usbpacket::IN, 1000);

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