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
#include "pins.h"
#include "time.h"

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

// Setup function gets called first
extern void setup(void);

// loop function gets called from a while loop in main
extern void loop(void);

// Cortex M4 Assembly LDR PC is 5 cycles. Check with the professor?
// 2-2-20 2x LDR Rx,[PC,#imm] (5 or 6), 1x LDR Rx,[Rx,#imm] (3), 1x STR Rx,[Ry,#imm] (1)
// This operation takes at least 9 cycles
typedef void (*GPIOPort_ISR)(void);


// Sets up a gpio pin. This is an unprotected function because I'm honestly lazy.
// ToDo: Proper data bits protections
void pinMode(uint8_t pin, uint8_t mode);

/**
 * Setup a pin for interrupts
 * RISING - a change from LOW TO HIGH generates an interrupt
 * FALLING - a change from HIGH to LOW generatesn an interrupt
 * CHANGE - both RISING and FALLING
 * LOW - an interrupt is triggered whenever the pin is LOW
 * HIGH - an interrupt is triggered whenever the pin is HIGH
 */
void attachInterrupt(uint8_t pin, void (*ISR)(void), uint8_t mode, uint32_t priority);

// Set a pin to low or high
void digitalWrite(uint8_t pin, uint8_t data);

// Read a pin, returns low or high
uint8_t digitalRead(uint8_t pin);

/**
 * GPIO Handlers
 * Automatically handle the GPIO port and allow Interrupt Service Routines
 * for each pins
 * 
 * ToDo: Perhaps make these __attribute__((weak))?
 */
void GPIOPort_Handler(uint8_t portnum, uint8_t portsize);
void GPIOPortA_Handler(void);
void GPIOPortB_Handler(void);
void GPIOPortC_Handler(void);
void GPIOPortD_Handler(void);
void GPIOPortE_Handler(void);
void GPIOPortF_Handler(void);

#endif /* TM4C123G_H */
