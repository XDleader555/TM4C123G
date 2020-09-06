/*
  pins.h - Pin definition for the TM4C123G

  Copyright (c) 2020 Andrew Miyaguchi. All rights reserved.

  Loosely based on the arduino environment for simplifying the pin process

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

#ifndef PINS_H
#define PINS_H

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stddef.h>

#define pwmgen_to_pwmmod(pwm_gen) ((pwm_mod < 9) ? 0 : 1)
#define pwmgen_to_pwmmodaddr(pwm_gen) ((pwm_mod < 9) ? &PWM0_0_CTL_R : &PWM1_0_CTL_R)

extern volatile unsigned long * port_addrs[];
extern volatile uint8_t pin_to_pwm_gen[];
extern volatile uint8_t pin_to_port[];
extern volatile uint8_t pin_to_pin_mask[];
extern volatile uint8_t pin_to_port_bit[];
extern volatile uint8_t port_to_port_mask[];

// PWM modulators
#define NOPWM   0
#define M0PWM0  1
#define M0PWM1  2
#define M0PWM2  3
#define M0PWM3  4
#define M0PWM4  5
#define M0PWM5  6
#define M0PWM6  7
#define M0PWM7  8
#define M1PWM0  9
#define M1PWM1  10
#define M1PWM2  11
#define M1PWM3  12
#define M1PWM4  13
#define M1PWM5  14
#define M1PWM6  15
#define M1PWM7  16

// Ports
#define PA 0
#define PB 1
#define PC 2
#define PD 3
#define PE 4
#define PF 5

// Pins (43 in total)
#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7
#define PB0 8
#define PB1 9
#define PB2 10
#define PB3 11
#define PB4 12
#define PB5 13
#define PB6 14
#define PB7 15
#define PC0 16
#define PC1 17
#define PC2 18
#define PC3 19
#define PC4 20
#define PC5 21
#define PC6 22
#define PC7 23
#define PD0 24
#define PD1 25
#define PD2 26
#define PD3 27
#define PD4 28
#define PD5 29
#define PD6 30
#define PD7 31
#define PE0 32
#define PE1 33
#define PE2 34
#define PE3 35
#define PE4 36
#define PE5 37
#define PF0 38
#define PF1 39
#define PF2 40
#define PF3 41
#define PF4 42

// Port data offsets
#define P_DATA      ((unsigned long) 0x3FC)
#define P_DIR       ((unsigned long) 0x400)
#define P_IS        ((unsigned long) 0x404)
#define P_IBE       ((unsigned long) 0x408)
#define P_IEV       ((unsigned long) 0x40C)
#define P_IM        ((unsigned long) 0x410)
#define P_RIS       ((unsigned long) 0x414)
#define P_MIS       ((unsigned long) 0x418)
#define P_ICR       ((unsigned long) 0x41C)
#define P_AFSEL     ((unsigned long) 0x420)
#define P_DR2R      ((unsigned long) 0x500)
#define P_DR4R      ((unsigned long) 0x504)
#define P_DR8R      ((unsigned long) 0x508)
#define P_ODR       ((unsigned long) 0x50C)
#define P_PUR       ((unsigned long) 0x510)
#define P_PDR       ((unsigned long) 0x514)
#define P_SLR       ((unsigned long) 0x518)
#define P_DEN       ((unsigned long) 0x51C)
#define P_LOCK      ((unsigned long) 0x520)
#define P_CR        ((unsigned long) 0x524)
#define P_AMSEL     ((unsigned long) 0x528)
#define P_PCTL      ((unsigned long) 0x52C)
#define P_ADCCTL    ((unsigned long) 0x530)
#define P_DMACTL    ((unsigned long) 0x534)

#endif /* PINS_H */
