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

#include <stdint.h>
#include "include/tm4c123gh6pm.h"
#include "pins.h"

// These are 8 bit functions
#define clrbit(data, mask) (data &= ~mask)
#define setbit(data, mask) (data |= mask)

// port data macro for functions
#define portData(port_offset, data_offset) (*(port_offset + (data_offset / sizeof(port_offset))))

#define OUTPUT 0
#define INPUT 1
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3

#define LOW 0
#define HIGH 1

// Page 710 of the datasheet states 80MHz clock (12.5 ns)
volatile unsigned long timer0_micros = 0;

// Cortex M4 Assembly LDR PC is 5 cycles. Check with the professor?
// 2-2-20 2x LDR Rx,[PC,#imm] (5 or 6), 1x LDR Rx,[Rx,#imm] (3), 1x STR Rx,[Ry,#imm] (1)
// This operation takes at least 9 cycles
volatile unsigned long clockDelay;

void init() {
  SYSCTL_RCGCTIMER_R |= 0x01;     // 0) activate TIMER0
  clockDelay = SYSCTL_RCGCTIMER_R;// delay by assigning a register
  TIMER0_CTL_R    = 0x00000000;   // 1) disable TIMER0A during setup
  TIMER0_CFG_R    = 0x00000004;   // 2) configure for 16-bit mode
  TIMER0_TAMR_R   = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R  = 0x00000050;   // 4) count 80 clocks, or 1 us
  TIMER0_TAPR_R   = 0x00000000;   // 5) no prescaler
  TIMER0_ICR_R    = 0x00000001;   // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R    = 0x00000001;   // 7) arm timeout interrupt
  TIMER0_CTL_R    = 0x00000001;   // 8) enable TIMER0A
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;  // acknowledge TIMER0A timeout
  timer0_micros ++;
}

unsigned long millis() {
  return timer0_micros / 1000;
}

unsigned long micros() {
  return timer0_micros;
}

// ToDo: Finish this function, figureout how long bitshifting takes
// void delayMicros(uint16_t micros) {
//   // return immediately if zero
//   if(!micros)
//     return; 
// }

// Sets up a gpio pin. This is an unprotected function because I'm honestly lazy.
// ToDo: Proper data bits protections
void pinMode(uint8_t pin, uint8_t mode) {
  volatile unsigned long * port = ports[pin / 10];
	uint8_t port_mask = (((uint8_t) 1) << (pin / 10));
  uint8_t pin_mask = (((uint8_t) 1) << (pin % 10));

  SYSCTL_RCGC2_R |= ((uint64_t) port_mask);   // gpio clock
  clockDelay = SYSCTL_RCGC2_R;                // delay by assigning a register
	if(pin == PC0 || pin == PC1 || pin == PC2 || pin == PD7 || pin == PF0) {
		portData(port, P_LOCK)   =  0x4C4F434B;     // unlock port
	}
  setbit(portData(port, P_CR), pin_mask);     // allow changes to pin
	clrbit(portData(port, P_AMSEL), pin_mask);  // Force digital instead of analog
  clrbit(portData(port, P_PCTL), pin_mask);   // GPIO clear bit PCTL
  
  clrbit(portData(port, P_PUR), pin_mask);    // clear pullup bit
  clrbit(portData(port, P_PDR), pin_mask);    // clear pulldown bit

  if(mode == INPUT) {
    clrbit(portData(port, P_DIR), pin_mask);  // Set Direction as input

    if(mode == INPUT_PULLUP) {
      setbit(portData(port, P_PUR), pin_mask);  // enable pullup resistor
    }

    if(mode == INPUT_PULLDOWN) {
      setbit(portData(port, P_PDR), pin_mask);
    }
  } else {
    setbit(portData(port, P_DIR), pin_mask);  // Set Direction as output
  }
  
  clrbit(portData(port, P_AFSEL), pin_mask);  // Regular IO
  setbit(portData(port, P_DEN), pin_mask);    // enable digital pins PF4-PF0
}

// Set a pin to low or high
void digitalWrite(uint8_t pin, uint8_t data) {
  volatile unsigned long * port = ports[pin / 10];
  uint8_t pin_mask = (((uint8_t) 1) << (pin % 10));

  if(data) {
    setbit(portData(port, P_DATA), pin_mask);
  } else {
    clrbit(portData(port, P_DATA), pin_mask);
  }
}

// Read a pin, returns low or high
uint8_t digitalRead(uint8_t pin) {
  volatile unsigned long * port = ports[pin / 10];
  uint8_t pin_mask = (((uint8_t) 1) << (pin % 10));

  if(portData(port, P_DATA) & pin_mask) {
    return HIGH;
  }
  
  return LOW;
}

#endif /* TM4C123G_H */
