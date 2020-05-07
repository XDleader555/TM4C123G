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

#ifndef TIME_H
#define TIME_H

#include <stdint.h>
#include <stddef.h>
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"
#include "timer.h"

#define INIT_TIMER		0
#define INIT_SYSTICK	1

/**
 * Setup systick for millis() and micros() usage
 * Set the PLL to 80Mhz and reload every 80 ticks
 */
void SysTick_Time_Init(void);

/**
 * Setup a timer for use with millis() and micros()
 * @param timer timer number to initialize
 */
void Timer_Time_Init(uint8_t timer);

/**
 * Return the time since initialization in milliseconds
 */
unsigned long millis(void);

/**
 * Returns the time since initialization in microseconds
 */
unsigned long micros(void);

/**
 * No operation for the specified number of milliseconds
 * @param ms number of milliseconds to delay
 */
void delay(unsigned long ms);

/**
 * No operation for the specified number of microseconds
 * @param us number of microseconds to delay
 */
void delayMicros(uint16_t us);

#endif /* TIME_H */
