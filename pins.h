/*
  pins.h - Pin definition for the TM4C123G

  Copyright (c) 2020 Andrew Miyaguchi. All rights reserved.

  Based on the arduino environment for simplifying the pin process

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

// Arbitray numbers, the first being the port, and the second being the gpio
#define PA0 (00)
#define PA1 (01)
#define PA2 (02)
#define PA3 (03)
#define PA4 (04)
#define PA5 (05)
#define PA6 (06)
#define PA7 (07)

#define PB0 (10)
#define PB1 (11)
#define PB2 (12)
#define PB3 (13)
#define PB4 (14)
#define PB5 (15)
#define PB6 (16)
#define PB7 (17)

#define PC0 (20)
#define PC1 (21)
#define PC2 (22)
#define PC3 (23)
#define PC4 (24)
#define PC5 (25)
#define PC6 (26)
#define PC7 (27)

#define PD0 (30)
#define PD1 (31)
#define PD2 (32)
#define PD3 (33)
#define PD4 (34)
#define PD5 (35)
#define PD6 (36)
#define PD7 (37)

#define PE0 (40)
#define PE1 (41)
#define PE2 (42)
#define PE3 (43)
#define PE4 (44)
#define PE5 (45)
#define PE6 (46)
#define PE7 (47)

#define PF0 (50)
#define PF1 (51)
#define PF2 (52)
#define PF3 (53)
#define PF4 (54)

// Port offsets
const volatile unsigned long * ports[] {
    GPIO_PORTA_DATA_BITS_R,
    GPIO_PORTB_DATA_BITS_R,
    GPIO_PORTC_DATA_BITS_R,
    GPIO_PORTD_DATA_BITS_R,
    GPIO_PORTE_DATA_BITS_R,
    GPIO_PORTF_DATA_BITS_R
};

// Port data offsets
#define P_DATA      (0x3FC)
#define P_DIR       (0x400)
#define P_IS        (0x404)
#define P_IBE       (0x408)
#define P_IEV       (0x40C)
#define P_IM        (0x410)
#define P_RIS       (0x414)
#define P_MIS       (0x418)
#define P_ICR       (0x41C)
#define P_AFSEL     (0x420)
#define P_DR2R      (0x500)
#define P_DR4R      (0x504)
#define P_DR8R      (0x508)
#define P_ODR       (0x50C)
#define P_PUR       (0x510)
#define P_PDR       (0x514)
#define P_SLR       (0x518)
#define P_DEN       (0x51C)
#define P_LOCK      (0x520)
#define P_CR        (0x524)
#define P_AMSEL     (0x528)
#define P_PCTL      (0x52C)
#define P_ADCCTL    (0x530)
#define P_DMACTL    (0x534)

// port data macro for functions
volatile unsigned long portData(volatile unsigned long * port_offset, unsigned long data_offset) {
    return *(port_offset + data_offset);
};

#endif /* PINS_H */