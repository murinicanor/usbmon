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

#define MON_IOC_MAGIC	 0x92
#define MON_IOCX_GETX	 _IOW(MON_IOC_MAGIC, 10, usbpacket::usbmon_get)
#define MON_IOCG_STATS   _IOR(MON_IOC_MAGIC, 3, usbpacket::mon_bin_stats)

enum CallBackMessage{
	BROKEN_RULE, THREAD_FAILS, OVERFLOW_COUNTER
};

class Rule{

public:
	Rule(uint16_t busnum, unsigned char devnum, usbpacket::Direction direction, uint64_t data_limit);

	uint64_t getID();
	unsigned char getDeviceNumber();
	uint16_t getBusNumber();
	usbpacket::Direction getDirection();
	uint64_t getTransferedData();
	uint64_t getDataTransferLimit();
	
	void setDeviceNumber(unsigned char num);
	void setBusNumber(uint16_t num);
	void setDirection(usbpacket::Direction direction);
	void setDataTransferLimit(uint64_t limit);

	void addTransferedData(uint64_t add);

	~Rule();

private:
	std::mutex mtx;

	uint64_t id;
	unsigned char devnum;
	uint16_t busnum;
	usbpacket::Direction direction;
	uint64_t transfered_data;
	uint64_t data_limit;

	uint64_t generateNewId();
	
};


class Usbmon{

public:
	Usbmon(void (*callback)(CallBackMessage, std::shared_ptr<usbmonitor::Rule>));

	int UsbmonInit(std::string usbmon_file_path);

	void monitorLoop();
	void waitThread();

	void setLoopState(bool state);
	bool getLoopState();

	void setFileDescriptor(int fd);
	int getFileDescriptor();

	uint64_t addRule(uint16_t busnum, unsigned char devnum,	usbpacket::Direction direction, uint64_t data_limit);
	int removeRule(uint64_t rule_id);
	void clearRules();

	int modifyRuleBusnum(uint64_t rule_id, uint16_t busnum);
	int modifyRuleDevnum(uint64_t rule_id, unsigned char devnum);
	int modifyRuleDirection(uint64_t rule_id, usbpacket::Direction direction);
	int modifyRuleDataLimit(uint64_t rule_id, uint64_t data_limit);

	int getNumOfRules();

	~Usbmon();

private:

	void (*callback)(CallBackMessage, std::shared_ptr<usbmonitor::Rule>);

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