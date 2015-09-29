// --usbmonitor--
//
// Copyright (C) 2015  Radovan Sroka
//
// This file is part of usbmonitor.
//
// UsbMonitor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
	/**
	 * Rule Constructor
	 *
	 * This method creates new Rule
	 *
	 * @param  busnum Bus Number
	 * @param  devnum Device Number
	 * @param  direction Direction of watched packets usbpacket::IN|OUT|BOTH
	 * @param  data_limit Rule limit in bytes
	 */
	Rule(uint16_t busnum, unsigned char devnum, usbpacket::Direction direction, uint64_t data_limit);
	
	/**
	 * Rule ID getter
	 *
	 * This method gives access to ID
	 *
	 * @return Returns ID of Rule
	 */
	uint64_t getID();

	/**
	 * Rule Device Number getter
	 *
	 * This method gives access to Device Number of packet in the Rule
	 *
	 * @return Returns Device Number of USB Device 
	 */
	unsigned char getDeviceNumber();

	/**
	 * Rule Bus Number getter
	 *
	 * This method gives access to Bus Number of packet in the Rule
	 *
	 * @return Returns Bus Number of USB Device
	 */
	uint16_t getBusNumber();

	/**
	 * Rule Direction getter
	 *
	 * This method gives access to Direction of watched packets
	 *
	 * @return Returns Direction of USB packet
	 */
	usbpacket::Direction getDirection();
	
	/**
	 * Rule Transfered data getter
	 *
	 * This method gives number of transfered bytes in the Rule 
	 *
	 * @return Returns number of transfered bytes
	 */
	uint64_t getTransferedData();
	
	/**
	 * Rule Transfer data limit getter
	 *
	 * This method gives transfer data limit of Rule in bytes 
	 *
	 * @return Returns data limit in bytes
	 */
	uint64_t getDataTransferLimit();
	
	/**
	 * Rule Device number setter
	 *
	 * This method sets Rule Device Number
	 *
	 * @param num Device Number to set
	 */
	void setDeviceNumber(unsigned char num);

	/**
	 * Rule Bus number setter
	 *
	 * This method sets Rule Bus Number
	 *
	 * @param num Bus Number to set
	 */
	void setBusNumber(uint16_t num);

	/**
	 * Rule Direction setter
	 *
	 * This Method sets Rule Direction usbpacket::IN|OUT|BOTH
	 *
	 * @param direction Direction to set
	 */
	void setDirection(usbpacket::Direction direction);

	/**
	 * Rule DataTransferLimit setter
	 *
	 * This method sets Rule Data Tranfer Limit
	 *
	 * @param limit Data Transfer Limit to set
	 */
	void setDataTransferLimit(uint64_t limit);

	/**
	 * Add transfered data into Rule
	 *
	 * @param add Bytes to add 
	 */
	void addTransferedData(uint64_t add);

	/**
	 * Destructor of Rule
	 */
	~Rule();

private:
	std::mutex mtx;

	uint64_t id;
	unsigned char devnum;
	uint16_t busnum;
	usbpacket::Direction direction;
	uint64_t transfered_data;
	uint64_t data_limit;
		
};


class Usbmon{

public:
	/**
	 * Usbmon constructor
	 *
	 * CallBack Function has two parameters
	 * 	- CallBackMessage - BROKEN_RULE|THREAD_FAILS|OVERFLOW_COUNTER
	 *	- shared_ptr to Rule
	 * @param callback Pointer to Callback function
	 */
	Usbmon(void (*callback)(CallBackMessage, std::shared_ptr<usbmonitor::Rule>));

	/**
	 * Initialization of Usbmon
	 *
	 * @param usbmon_file_path Path usbmon api file 
	 *
	 * @return EXIT_SUCCESS | EXIT_FAILURE
	 */
	int UsbmonInit(std::string usbmon_file_path);

	/**
	 * Start Monitor Loop and Thread
	 */
	void monitorLoop();

	/**
	 * Wait and Sync with monitor Thread
	 */
	void waitThread();

	/**
	 * Set Loop State
	 *
	 * Loop is active until state is false, but after that 
	 * set state to true doesn't mean run thread again
	 *
	 * In that case it's better to call usbmoninit again
	 *
	 * @param state State loop to set
	 */
	void setLoopState(bool state);
	
	/**
	 * Loop state getter
	 *
	 * @return Returns Loop State
	 */
	bool getLoopState();

	//void setFileDescriptor(int fd);
	//int getFileDescriptor();

	/**
	 * Add new Rule into Usbmon
	 *
	 * In this method is created entire new Rule and ID
	 *
	 * @param busnum Bus Number of USB device
	 * @param devnum Device Number of USB device
	 * @param direction Direction watched packets
	 * @param data_limit Data limit of the new Rule
	 *
	 * @return Returns ID of new Rule
	 */
	uint64_t addRule(uint16_t busnum, unsigned char devnum,	usbpacket::Direction direction, uint64_t data_limit);
	
	/**
	 * Remove Rule
	 *
	 * If Rule with rule_id exist than that rule is removed and returned EXIT_SUCCESS
	 * Otherwise this method returns EXIT_FAILURE
	 *
	 * @param rule_id ID of Rule to Remove
	 *
	 * @return EXIT_SUCCESS | EXIT_FAILURE
	 */
	int removeRule(uint64_t rule_id);

	/**
	 * Clear all Rules
	 */
	void clearRules();

	/**
	 * Modify Rule Bus Number
	 *
	 * If Rule with rule_id exist than that rule is modified and returned EXIT_SUCCESS
	 * Otherwise this method returns EXIT_FAILURE
	 *
	 *
	 * @param rule_id ID of Rule to modify
	 * @param busnum New Bus Number to set
	 *
	 * @return EXIT_SUCCESS | EXIT_FAILURE
	 */
	int modifyRuleBusnum(uint64_t rule_id, uint16_t busnum);

	/**
	 * Modify Device Number
	 *
	 * If Rule with rule_id exist than that rule is modified and returned EXIT_SUCCESS
	 * Otherwise this method returns EXIT_FAILURE
	 *
	 * @param rule_id ID of Rule to modify
	 * @param devnum New Device Number to set
	 *
	 * @return EXIT_SUCCESS | EXIT_FAILURE
	 */
	int modifyRuleDevnum(uint64_t rule_id, unsigned char devnum);

	/**
	 * Modify Rule Direction
	 *
	 * If Rule with rule_id exist than that rule is modified and returned EXIT_SUCCESS
	 * Otherwise this method returns EXIT_FAILURE
	 *
	 * @param rule_id ID of Rule to modify
	 * @param direction New Direction to set
	 *
	 * @return EXIT_SUCCESS | EXIT_FAILURE
	 */
	int modifyRuleDirection(uint64_t rule_id, usbpacket::Direction direction);

	/**
	 * Modify Rule Data Limit
	 *
	 * If Rule with rule_id exist than that rule is modified and returned EXIT_SUCCESS
	 * Otherwise this method returns EXIT_FAILURE
	 *
	 * @param rule_id ID of Rule to modify
	 * @param data_limit New Data Limit to set
	 *
	 * @return EXIT_SUCCESS | EXIT_FAILURE
	 */
	int modifyRuleDataLimit(uint64_t rule_id, uint64_t data_limit);

	/**
	 * Get Number of Rules
	 *
	 * @return Number of Rules
	 */
	int getNumOfRules();

	/**
	 * Check if Print Mode is enabled
	 *
	 * @return true|false
	 */
	bool isPrintSet();

	/**
	 * Enable/Disable Print Mode
	 *
	 * @param set May be true as enable or false as disable
	 */
	void setPrint(bool set);

	/**
	 * Usbmon destructor
	 */
	~Usbmon();

private:

	void (*callback)(CallBackMessage, std::shared_ptr<usbmonitor::Rule>);

	std::mutex mtx;
	std::thread *monitorThread;
	std::list<std::shared_ptr<Rule>> * rules;

	std::string usbmon_file_path;
	int usbmon_fd;
	bool loopstate;
	bool print;

	std::shared_ptr<Rule> * getRule(uint64_t rule_id);
	void applyRules(usbpacket::UsbPacket * packet);
	void checkRules();
	int loop();
};

}

#endif
