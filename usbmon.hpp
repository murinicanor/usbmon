#ifndef USBMON_HPP
#define USBMON_HPP

#include <iostream>
#include <mutex>
#include <map>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include "usbpacket.hpp"


#define MON_IOC_MAGIC	0x92
#define MON_IOCX_GET	_IOW(MON_IOC_MAGIC, 6, struct usbmon_get)
#define MON_IOCX_GETX	_IOW(MON_IOC_MAGIC, 10, struct usbmon_get)


class Usbmon
{
public:
	Usbmon();
	~Usbmon();

private:
	std::mutex mtx;
	std::map<int, long long int> stats;

};

#endif