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

	usbmon->setPrint(true);

	uint64_t first = usbmon->addRule(1,10, usbpacket::BOTH, 100);
	uint64_t second = usbmon->addRule(1,9, usbpacket::IN, 1000);

	std::cout << "Number of rules = " << usbmon->getNumOfRules() << std::endl;

	char c = ' ';
	while(c != 'q'){
		std::cin >> c;
		usbmon->setLoopState(false);
	}

	usbmon->removeRule(first);
	usbmon->removeRule(second);

	usbmon->waitThread();

	delete usbmon;

	return 0;
}
