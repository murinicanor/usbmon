#include <iostream>
#include <thread>
#include <ctime>
#include <unistd.h>

#include "usbmon.hpp"

using namespace std;
Usbmon * usbmon;

int main(int argc, char const *argv[])
{
	srand(time(0));
	clock_t init_time = clock();
	usbmon = new Usbmon();
	usbmon->UsbmonInit("/dev/usbmon0");

	thread loop(&Usbmon::monitorLoop, usbmon);

	sleep(5);
	usbmon->setLoopState(false);
	loop.join();

	delete usbmon;

	return 0;
}