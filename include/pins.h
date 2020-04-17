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

extern volatile unsigned long * ports[];

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

#define PF0 (50)
#define PF1 (51)
#define PF2 (52)
#define PF3 (53)
#define PF4 (54)

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
