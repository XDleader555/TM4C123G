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

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#include <stddef.h>
#include "tm4c123gh6pm.h"

/**
 * Friendly systick initialization function. Expanded to allow configuration
 * of pll, handler, and reload value
 * @param pllinit PLL Init handler
 * @param handler systick overflow handler
 * @param reload  systick overflow count
 * @param priority interrupt priority
 */
void SysTick_Init_Custom(void (*pllinit)(void), void (*handler)(void), uint32_t reload, uint32_t priority);

void SysTick_Handler(void);

#endif /* SYSTICK_H */
