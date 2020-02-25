/*
  SysTick.c - Timing functions based on systick for the TM4C123G
	
	Function to initialize the systick timer and delay functions
	Based on Daniel Valvano's SysTick.h
	
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

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PLL.h"

// Page 710 of the datasheet states 80MHz clock (12.5 ns)
volatile unsigned long timer0_micros = 0;

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void);

void SysTick_Handler(void);

unsigned long millis(void);

unsigned long micros(void);

void delay(unsigned long ms);

void delayMicros(uint16_t us);