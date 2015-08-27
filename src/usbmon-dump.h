/* Based on Documentation/usbmon.txt in the Linux Kernel */

/* usbmon-dump.h - based on usbmon.h
*
* Copyright 2011 Brian Swetland <swetland@frotz.net>
* Copyright 2015 Radovan Sroka <rsroka@redhat.com> 
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>


#include <sys/ioctl.h>
#include <linux/ioctl.h>



#ifndef _USBMON_H_
#define _USBMON_H_


struct usbmon_packet {
	uint64_t id;			/* URB ID */
	unsigned char type;	/* 'S'ubmit 'C'allback 'E'rror */
	unsigned char xfer;	/* ISO=0 INT=1 CTRL=2 BULK=3 */
	unsigned char epnum;
	unsigned char devnum;
	uint16_t busnum;
	char flag_setup;
	char flag_data;
	int64_t ts_sec;
	int32_t ts_usec;
	int32_t status;
	uint32_t length;
	uint32_t len_cap;
	union {
		unsigned char setup[8];
		struct iso_rec {
			int32_t error_count;
			int32_t numdesc;
		} iso;
	} s;
	int32_t interval;
	int32_t start_frame;
	uint32_t xfer_flags;
	uint32_t ndesc;
};

struct usbmon_get {
	struct usbmon_packet *hdr;
	void *data;
	size_t alloc;
};

#define MON_IOC_MAGIC	0x92

#define MON_IOCX_GET	_IOW(MON_IOC_MAGIC, 6, struct usbmon_get)
#define MON_IOCX_GETX	_IOW(MON_IOC_MAGIC, 10, struct usbmon_get)

#endif
