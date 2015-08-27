# --usbmonitor--
#
#  Copyright (C) 2015  Radovan Sroka
#
#  This file is part of usbmonitor.
#
#  UsbMonitor is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

AM_CFLAGS = -O2 -g -Wall -Wextra -pedantic -std=c99
AM_CXXFLAGS = -O2 -g -Wall -Wextra -pedantic -std=c++11
AM_LDFLAGS = -pthread

sbin_PROGRAMS = usbmondump usbmonapi
usbmondump_SOURCES = usbmon-dump.c usbmon-dump.h
usbmonapi_SOURCES = usbmon-api.cpp usbmon.cpp usbpacket.cpp usbmon.hpp usbmonpacket.hpp
