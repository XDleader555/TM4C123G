/*
  util.h - preprocessor directives and common functions

  Copyright (c) 2020 Andrew Miyaguchi. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef UTIL_H
#define UTIL_H

// These are 8 bit functions
#define clrbit(data, mask) (data &= ~mask)
#define setbit(data, mask) (data |= mask)

// port data macro for functions
#define portDataAddr(port_offset, data_offset)  (port_offset + (data_offset / sizeof(port_offset)))
#define portData(port_offset, data_offset)      (*portDataAddr(port_offset, data_offset))

// Min and max definitions from stl, but like without stl ;)
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

#endif /* UTIL_H */
