/*
  SysTick.c - Timing functions based on systick for the TM4C123G
	
	Function to initialize the systick timer and delay functions
	Based on Daniel Valvano's SysTick.c
	
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

#include "SysTick.h"

void (*_SysTick_Handler)(void);

void SysTick_Init_Custom(void (*pllinit)(void), void (*handler)(void), uint32_t reload, uint32_t priority) {
  // Init the PLL if requested
  if(pllinit != NULL)
    (*pllinit)();
  
  // Assign internal handler for when systick overflows
  _SysTick_Handler = handler;

  // Initialize the systick
  NVIC_ST_CTRL_R = 0x00;						// disable systick during setup
  NVIC_ST_RELOAD_R = reload - 1;    		// count 80 ticks, or 1 us, dont forget we're indexed at zero
  NVIC_ST_CURRENT_R = 0;            // Set to zero so we start at the max value
  NVIC_SYS_PRI3_R &= ~(((uint32_t) (0x00000007)) << 29);          // clear priority
  NVIC_SYS_PRI3_R |= (priority & 0x00000007) << 29; // Set priority
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE + NVIC_ST_CTRL_CLK_SRC + NVIC_ST_CTRL_INTEN;  // Use system clock
}

/**
 * Call the systick handler helper function which should always be set by
 * systick init. We avoid the null pointer check to reduce overhead
 */
void SysTick_Handler(void){
  (*_SysTick_Handler)();
}
