#ifndef USBMON_HPP
#define USBMON_HPP

#include <iostream>
#include <string>
#include <mutex>
#include <list>
#include <memory>
#include <thread>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include "usbpacket.hpp"

namespace usbmonitor{

#define MON_IOC_MAGIC	0x92
#define MON_IOCX_GET	_IOW(MON_IOC_MAGIC, 6,  usbpacket::usbmon_get)
#define MON_IOCX_GETX	_IOW(MON_IOC_MAGIC, 10, usbpacket::usbmon_get)

class Rule
{
public:
	Rule(uint16_t busnum, unsigned char devnum, usbpacket::Direction direction, intmax_t data_limit);

	uint64_t getID();
	unsigned char getDeviceNumber();
	uint16_t getBusNumber();
	usbpacket::Direction getDirection();
	uintmax_t getTransferedData();
	intmax_t getDataTransferLimit();
	
	void setDeviceNumber(unsigned char num);
	void setBusNumber(uint16_t num);
	void setDirection(usbpacket::Direction direction);
	void setDataTransferLimit(intmax_t limit);

	void addTransferedData(uintmax_t add);

	~Rule();

private:
	uint64_t id;
	unsigned char devnum;
	uint16_t busnum;
	usbpacket::Direction direction;
	uintmax_t transfered_data;
	intmax_t data_limit;
	
};


class Usbmon
{
public:
	Usbmon();

	int UsbmonInit(std::string usbmon_file_path);

	void monitorLoop();
	void waitThread();

	void setLoopState(bool state);
	bool getLoopState();

	void setFileDescriptor(int fd);
	int getFileDescriptor();

	uint64_t addRule(uint16_t busnum, unsigned char devnum,	usbpacket::Direction direction, intmax_t data_limit);
	int removeRule(uint64_t rule_id);
	void clearRules();

	int modifyRule(uint64_t rule_id);

	int getNumOfRules();

	~Usbmon();

private:
	std::mutex mtx;
	std::thread *monitorThread;
	std::list<std::shared_ptr<Rule>> * rules;

	std::string usbmon_file_path;
	int usbmon_fd;
	bool loopstate;

	std::shared_ptr<Rule> * getRule(uint64_t rule_id);
	void applyRules(usbpacket::UsbPacket * packet);
	void checkRules();
	int loop();
};

}

#endif