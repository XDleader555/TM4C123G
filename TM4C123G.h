	/*
  TM4C123G.h - Development functions and macros for the TM4C123G

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

#ifndef TM4C123G_H
#define TM4C123G_H

#include "tm4c123gh6pm.h"
// These are 8 bit functions
#define clrbit(data, mask) (data &= ~mask)
#define setbit(data, mask) (data |= mask)

// port data macro for functions
#define portDataAddr(port_offset, data_offset)  (port_offset + (data_offset / sizeof(port_offset)))
#define portData(port_offset, data_offset)      (*portDataAddr(port_offset, data_offset))

extern void DisableInterrupts(void); // Disable interrupts
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // low power mode

#include "gpio_digital.h"
#include "pwm.h"
#include "time.h"

#endif /* TM4C123G_H */
