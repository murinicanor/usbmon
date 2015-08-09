#include <iostream>
#include <thread>
#include <unistd.h>

#include "usbpacket.hpp"
#include "usbmon.hpp"

using namespace std;
Usbmon * usbmon;

int main(int argc, char const *argv[])
{
	
	usbmon = new Usbmon();
	usbmon->UsbmonInit("/dev/usbmon0");

	thread loop(&Usbmon::monitorLoop, usbmon);

	uint64_t first = usbmon->addRule(1,5, BOTH, 500);
	uint64_t second = usbmon->addRule(1,2, IN, 1000);

	std::cout << usbmon->getNumOfRules() << std::endl;

	//usbmon->removeRule(first);
	//usbmon->removeRule(second);

	std::cout << usbmon->getNumOfRules() << std::endl;

	sleep(5);
	usbmon->setLoopState(false);
	loop.join();



	delete usbmon;

	return 0;
}