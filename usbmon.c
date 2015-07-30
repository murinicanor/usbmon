/* usbmon.c
 *
 * Copyright 2011 Brian Swetland <swetland@frotz.net>
 * Copyright 2015 Radovan Sroka <rsroka@redhat.com> 
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "usbmon.h"
#define MASK_SIZE 50


struct Params{
	int b;
	int d;
	int x;
};

struct Params params = {.b=0, .d=0, .x=0}; 
static char _xfer[4] = "SICB";

int parse_parameters(int argc, const char **argv){


	argv++;
	argc--;	

	char mask[MASK_SIZE] = {0};

	if(argc > MASK_SIZE)return 1;

	for(int i = 0 ; i < argc ; i++){
		if(*argv[i] == '-'){
			mask[i] = 1;
			argv[i]++;
		}
	}

	for(int i = 0 ; i < argc ; i++){
		if(mask[i]){
			int * pointer = NULL;

			switch(*argv[i]){
				case 'b':
					pointer = &params.b;
					break;
				case 'd':
					pointer = &params.d;
					break;
				case 'x':
					pointer = &params.x;
					break;
				default:
					continue;
			}

			if(!mask[i+1] && i+1 < argc){
				char * ptr = NULL;
				*pointer = (int)strtol(argv[i+1], &ptr, 10);
				if(*ptr != '\0'){
					fprintf(stderr, "%s %d\n", "Cannot parse argument number", i+1);
					return 1;
				}
			}
			else {
				fprintf(stderr, "%s %d\n", "Cannot parse argument number", i+1);
				return 1;
			}
		}
	}

	return 0;

}


int main(int argc, const char **argv) 
{
	unsigned char data[4096];
	struct usbmon_packet hdr;
	struct usbmon_get arg;
	unsigned char filter_dev[128];
	int fd, r, n;
	unsigned busmask = 0;

	memset(filter_dev, 0, sizeof(filter_dev));

	if ((fd = open("/dev/usbmon0", O_RDONLY)) < 0){
		fprintf(stderr, "%s\n", "Cannot open /dev/usbmon0");
		return 1;
	}

	if(parse_parameters(argc, argv))goto failure;

	if(params.b)busmask = ~(1 << params.b);
	
	if(params.d > 0 && params.d < 127){
		memset(filter_dev, 0x01, sizeof(filter_dev));
		filter_dev[params.d] = 0;
	}
	else if(params.d){
		fprintf(stderr, "%s\n", "Parameter is not in range 1 - 127");
		goto failure;
	}

	if(params.x > 0 && params.x < 127)filter_dev[params.x] = 1;
	else if(params.x){
		fprintf(stderr, "%s\n", "Parameter is not in range 1 - 127");
		goto failure;
	}


	arg.hdr = &hdr;
	arg.data = data;
	for (;;) {
		arg.alloc = sizeof(data);
		r = ioctl(fd, MON_IOCX_GET, &arg);
		if (r < 0)
			break;
		if (filter_dev[hdr.devnum])
			continue;
		if (busmask & (1 << hdr.busnum))
			continue;
		printf("%d.%03d.%03d %c %c%c ------%04x",
			hdr.busnum, hdr.devnum, hdr.epnum & 0x7F,
			hdr.type,
			_xfer[hdr.xfer], (hdr.epnum & 0x80) ? 'i' : 'o',
#if 0
			hdr.flag_setup ? hdr.flag_setup : ' ',
			hdr.flag_data ? hdr.flag_data : ' ',
#endif
			hdr.length);
		if (hdr.type == 'S') {
			if (hdr.xfer == 2) {
				printf(" %02x %02x %02x%02x %02x%02x %02x%02x\n",
					hdr.s.setup[0], hdr.s.setup[1],
					hdr.s.setup[3], hdr.s.setup[2],
					hdr.s.setup[5], hdr.s.setup[4],
					hdr.s.setup[7], hdr.s.setup[6]);
			} else {
				goto dumpdata;
			}
	
		} else {
			switch (hdr.status) {
			case 0:
				printf(" OK\n");
				break;
			case -EPIPE:
				printf(" STALLED\n");
				break;
			case -ENODEV:
				printf(" DISCONNECTED\n");
				break;
			case -ETIMEDOUT:
				printf(" TIMEDOUT\n");
				break;
			case -EREMOTEIO:
				printf(" OK (SHORT)\n");
				break;
			default:
				printf(" %s (%d)\n", strerror(-hdr.status),-hdr.status);
			}
		}
		if (!hdr.len_cap) 
			continue;
		printf("                   ");
dumpdata:
		if (hdr.len_cap > sizeof(data))
			hdr.len_cap = sizeof(data);
		for (n = 0; n < (int)hdr.len_cap; n++) 
			printf((n & 3) ? " %02x" : " %02x",data[n]);
		printf("\n");
		fflush(stdout);
	}
	return 0;	

failure:
	close(fd);
	return 1;
}
