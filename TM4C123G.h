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
#include <stdbool.h>
#include <stddef.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"
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

#define RISING 2
#define FALLING 3
#define CHANGE 4

// Cortex M4 Assembly LDR PC is 5 cycles. Check with the professor?
// 2-2-20 2x LDR Rx,[PC,#imm] (5 or 6), 1x LDR Rx,[Rx,#imm] (3), 1x STR Rx,[Ry,#imm] (1)
// This operation takes at least 9 cycles
volatile unsigned long clockDelay;

// Sets up a gpio pin. This is an unprotected function because I'm honestly lazy.
// ToDo: Proper data bits protections
void pinMode(uint8_t pin, uint8_t mode) {
  volatile unsigned long * port = ports[pin / 10];
	uint8_t port_mask = (((uint8_t) 1) << (pin / 10));
  uint8_t pin_mask = (((uint8_t) 1) << (pin % 10));

  SYSCTL_RCGC2_R |= ((uint64_t) port_mask);   // gpio clock
  clockDelay = SYSCTL_RCGC2_R;                // delay by assigning a register
	portData(port, P_LOCK)   =  0x4C4F434B;     // unlock port
  setbit(portData(port, P_CR), pin_mask);     // allow changes to pin
	clrbit(portData(port, P_AMSEL), pin_mask);  // Force digital instead of analog
  clrbit(portData(port, P_PCTL), pin_mask);   // GPIO clear bit PCTL
  
  clrbit(portData(port, P_PUR), pin_mask);    // clear pullup bit
  clrbit(portData(port, P_PDR), pin_mask);    // clear pulldown bit

	if(mode == INPUT_PULLUP) {
		clrbit(portData(port, P_DIR), pin_mask);  // Set Direction as input
		setbit(portData(port, P_PUR), pin_mask);  // enable pullup resistor
	} else if(mode == INPUT_PULLDOWN) {
		clrbit(portData(port, P_DIR), pin_mask);  // Set Direction as input
		setbit(portData(port, P_PDR), pin_mask);
	} else {
    setbit(portData(port, P_DIR), pin_mask);  // Set Direction as output
  }
  
  clrbit(portData(port, P_AFSEL), pin_mask);  // Regular IO
  setbit(portData(port, P_DEN), pin_mask);    // enable digital pins PF4-PF0
}

// 
typedef void (*GPIOPort_ISR)(void);
GPIOPort_ISR GPIOPortA_ISR[8];
GPIOPort_ISR GPIOPortB_ISR[8];
GPIOPort_ISR GPIOPortC_ISR[8];
GPIOPort_ISR GPIOPortD_ISR[8];
GPIOPort_ISR GPIOPortE_ISR[6];
GPIOPort_ISR GPIOPortF_ISR[5];

GPIOPort_ISR *GPIOPortISR_List[] = {
    GPIOPortA_ISR,
    GPIOPortB_ISR,
    GPIOPortC_ISR,
    GPIOPortD_ISR,
    GPIOPortE_ISR,
    GPIOPortF_ISR
};

uint8_t GPIOPort_INT_NUM[] = {
  0,  // GPIO Port A
  1,  // GPIO Port B
  2,  // GPIO Port C
  3,  // GPIO Port D
  4,  // GPIO Port E
  30  // GPIO Port F
};

volatile unsigned long * GPIOPort_INT_PRI[] = {
    &NVIC_PRI0_R,
    &NVIC_PRI0_R,
    &NVIC_PRI0_R,
    &NVIC_PRI0_R,
    &NVIC_PRI1_R,
    &NVIC_PRI7_R
};

uint8_t GPIOPort_INT_PRI_OFFSET[] = {
    5,
    13,
    21,
    29,
    5,
    21
};

/**
 * Setup a pin for interrupts
 * RISING - a change from LOW TO HIGH generates an interrupt
 * FALLING - a change from HIGH to LOW generatesn an interrupt
 * CHANGE - both RISING and FALLING
 * LOW - an interrupt is triggered whenever the pin is LOW
 * HIGH - an interrupt is triggered whenever the pin is HIGH
 */
void attachInterrupt(uint8_t pin, void (*ISR)(void), uint8_t mode, uint32_t priority) {
  volatile unsigned long * port = ports[pin / 10];
	//uint8_t port_mask = (((uint8_t) 1) << (pin / 10));
  uint8_t pin_mask = (((uint8_t) 1) << (pin % 10));

  clrbit(portData(port, P_IM), pin_mask);  // Disable Interrupts during setup
  
  (GPIOPortISR_List[pin / 10])[pin % 10] = ISR; // Assign the ISR

  // Setup trigger
  if(mode == RISING) {
    clrbit(portData(port, P_IS), pin_mask);   // Interrupt Sense 0=edge-sensitive, 1=level-sensitive
    clrbit(portData(port, P_IBE), pin_mask);  // 1=Both edges, 0=Interrupt Event
    setbit(portData(port, P_IEV), pin_mask);  // Interrupt Event (1=Rising, 0=Falling)
  } else if(mode == FALLING) {
    clrbit(portData(port, P_IS), pin_mask);   // Interrupt Sense 0=edge-sensitive, 1=level-sensitive
    clrbit(portData(port, P_IBE), pin_mask);  // 1=Both edges, 0=Interrupt Event
    clrbit(portData(port, P_IEV), pin_mask);  // Interrupt Event (1=Rising, 0=Falling)
  } else if(mode == CHANGE) {
    clrbit(portData(port, P_IS), pin_mask);   // Interrupt Sense 0=edge-sensitive, 1=level-sensitive
    setbit(portData(port, P_IBE), pin_mask);  // 1=Both edges, 0=GPIOIEV
  } else if(mode == LOW) {
    setbit(portData(port, P_IS), pin_mask);   // Interrupt Sense 0=edge-sensitive, 1=level-sensitive
    clrbit(portData(port, P_IBE), pin_mask);  // 1=Both edges, 0=Interrupt Event
    clrbit(portData(port, P_IEV), pin_mask);  // Interrupt Event (1=HIGH, 0=LOW)
  } else if(mode == HIGH) {
    setbit(portData(port, P_IS), pin_mask);   // Interrupt Sense 0=edge-sensitive, 1=level-sensitive
    clrbit(portData(port, P_IBE), pin_mask);  // 1=Both edges, 0=Interrupt Event
    setbit(portData(port, P_IEV), pin_mask);  // Interrupt Event (1=HIGH, 0=LOW)
  }

  setbit(portData(port, P_RIS), pin_mask);  // Reset Raw Interrupt Status Flag
  setbit(portData(port, P_ICR), pin_mask);  // Reset Interrupt Flag
  setbit(portData(port, P_IM), pin_mask);  // Enable Interrupt

  // Find NVIC PRI from Page 152
	(*(GPIOPort_INT_PRI[pin/10])) &= ~(0x00000007 << GPIOPort_INT_PRI_OFFSET[pin/10]); // clear priority
  (*(GPIOPort_INT_PRI[pin/10])) |= ((priority & 0x00000007) << GPIOPort_INT_PRI_OFFSET[pin/10]); // write priority

  // Find interrupt Number from Page 104
  NVIC_EN0_R |= (0x01 << GPIOPort_INT_NUM[pin/10]); // Enable the interrupt
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

/**
 * GPIO Handlers
 * Automatically handle the GPIO port and allow Interrupt Service Routines
 * for each pins
 * 
 * ToDo: Perhaps make these __attribute__((weak))?
 */
void GPIOPort_Handler(uint8_t portnum, uint8_t portsize) {
  volatile unsigned long * port = ports[portnum];
  uint8_t isr;

  for(isr = 0; isr < portsize; isr ++)
    if((GPIOPortISR_List[portnum])[isr] != NULL)
      (*((GPIOPortISR_List[portnum])[isr]))();

  // Cleared after to allow for interrupt reason
  portData(port, P_ICR) = 0xFF;
}

void GPIOPortA_Handler(void) {
  GPIOPort_Handler(0, 8);
}

void GPIOPortB_Handler(void) {
  GPIOPort_Handler(1, 8);
}

void GPIOPortC_Handler(void) {
  GPIOPort_Handler(2, 8);
}

void GPIOPortD_Handler(void) {
  GPIOPort_Handler(3, 8);
}

void GPIOPortE_Handler(void) {
  GPIOPort_Handler(4, 6);
}

void GPIOPortF_Handler(void) {
  GPIOPort_Handler(5, 5);
}

#endif /* TM4C123G_H */
