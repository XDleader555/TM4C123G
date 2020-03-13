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

// Page 710 of the datasheet states 80MHz clock (12.5 ns)
volatile unsigned long timer0_micros;
void (*_SysTick_Handler)(void);

void micros_handle(void) {
  timer0_micros ++;
}

/**
 * Friendly systick initialization function. Expanded to allow configuration
 * of pll, handler, and reload value
 * @param pllinit PLL Init handler
 * @param handler systick overflow handler
 * @param reload  systick overflow count
 */
void SysTick_Init_Custom(void (*pllinit)(void), void (*handler)(void), uint32_t reload) {
  // Init the PLL if requested
  if(pllinit != NULL)
    (*pllinit)();
  
  // Assign internal handler for when systick overflows
  _SysTick_Handler = handler;

  // Initialize the systick
  NVIC_ST_CTRL_R = 0x00;						// disable systick during setup
  NVIC_ST_RELOAD_R = reload - 1;    		// count 80 ticks, or 1 us, dont forget we're indexed at zero
  NVIC_ST_CURRENT_R = 0;            // Set to zero so we start at the max value
  NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R & 0X00FFFFFF;  // Interrupt vector priority 0
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE + NVIC_ST_CTRL_CLK_SRC + NVIC_ST_CTRL_INTEN;  // Use system clock
}

/**
 * Automatically setup systick for millis() and micros() usage
 * Set the PLL to 80Mhz and reload every 80 ticks
 */
void SysTick_Init(void){
	timer0_micros = 0;								                // Clear our millis timer
  SysTick_Init_Custom(&(PLL_Init), &(micros_handle), 80);  // Init the systick
}

/**
 * Call the systick handler helper function which should always be set by
 * systick init. We avoid the null pointer check to reduce overhead
 */
void SysTick_Handler(void){
  (*_SysTick_Handler)();
}

unsigned long millis(void) {
  return timer0_micros / 1000;
}

unsigned long micros(void) {
  return timer0_micros;
}

void delay(unsigned long ms) {
  unsigned long startTime = millis();

  while(millis() - startTime <= ms) {} 
}

void delayMicros(uint16_t us) {
	unsigned long startTime;
  // return immediately if zero
  if(!us)
    return; 

  startTime = micros();
  
  // ToDo: Finish this function, figureout how long bitshifting takes
  while(micros() - startTime > us) {}
}
