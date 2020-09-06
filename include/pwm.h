/*
  pwm.h - Hardware Pulse Width Modulation for the TM4C123G

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

#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include <stddef.h>
#include "tm4c123gh6pm.h"
#include "pins.h"
#include "util.h"

#define PWM_RES             4095    // 12 bits of resolution to match ADC
static volatile uint32_t PWM_PERIOD = 40000;   // 1000hz, with an 80Mhz PLL

// Memory offsets used to configure each PWM module
#define PWM_CTL            ((unsigned long) 0x00)
#define PWM_INTEN          ((unsigned long) 0x04)
#define PWM_RIS            ((unsigned long) 0x08)
#define PWM_ISC            ((unsigned long) 0x0C)
#define PWM_LOAD           ((unsigned long) 0x10)
#define PWM_COUNT          ((unsigned long) 0x14)
#define PWM_CMPA           ((unsigned long) 0x18)
#define PWM_CMPB           ((unsigned long) 0x1C)
#define PWM_GENA           ((unsigned long) 0x20)
#define PWM_GENB           ((unsigned long) 0x24)
#define PWM_DBCTL          ((unsigned long) 0x28)
#define PWM_DBRISE         ((unsigned long) 0x2C)
#define PWM_DBFALL         ((unsigned long) 0x30)
#define PWM_FLTSRC0        ((unsigned long) 0x34)
#define PWM_FLTSRC1        ((unsigned long) 0x38)
#define PWM_MINFLTPER      ((unsigned long) 0x3C)

#define PWM_CFGSIZE        ((unsigned long) 0x40)


/**
 * Setup a pin for pwm
 * @param pin		gpio pin to write to, must use friendly definition (ex PF4)
 * @param data	value to write (0 - 4096)
 */
void analogWrite(uint8_t pin, uint16_t value);

/**
 * Sets the frequency for all pwm
 * Due to the nature of how pwm is setup, this will only affect the next call
 * of analogWrite. Additionally, it'll affect the any pwm output on the same
 * module.
 * @param freq  Desired frequency (Default: 1000hz)
 */
void analogWriteFrequency(uint16_t freq);
#endif /* PWM_H */
