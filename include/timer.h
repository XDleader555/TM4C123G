/*
  time.h - hardware clock responsible for millis and micros

  Copyright (c) 2020 Andrew Miyaguchi. All rights reserved.

  These functions can be used to setup the hardware clock on either
  the systick or timer. Normally timer0 is used to setup to keep track
  of time, but systick may also be used in the event that it is
  required.

  Systick may not accurately keep time as its interrupt priority may be
  lower than others.

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

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stddef.h>
#include "tm4c123gh6pm.h"

extern volatile unsigned long clockDelay;

/**
 * Friendly timer initialization function.
 * Expanded to allow configuration of handler and reload value
 * @param timer timer to initialize
 * @param handler systick overflow handler
 * @param reload  systick overflow count
 */
void Timer_Init_Custom(uint8_t timer, void (*handler)(void), uint32_t reload);

void Timer0A_Handler(void);

#endif /* TIMER_H */
