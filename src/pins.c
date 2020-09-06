#include "pins.h"

// Port offsets
volatile unsigned long * port_addrs[] = {
    GPIO_PORTA_DATA_BITS_R,
    GPIO_PORTB_DATA_BITS_R,
    GPIO_PORTC_DATA_BITS_R,
    GPIO_PORTD_DATA_BITS_R,
    GPIO_PORTE_DATA_BITS_R,
    GPIO_PORTF_DATA_BITS_R
};

// GPIO pin to generator
volatile uint8_t pin_to_pwm_gen[] = {
    NOPWM,  NOPWM,  NOPWM,  NOPWM,  NOPWM,  NOPWM,  M1PWM2, M1PWM3, // PA
    NOPWM,  NOPWM,  NOPWM,  NOPWM,  M0PWM2, M0PWM3, M0PWM0, M0PWM1, // PB
    NOPWM,  NOPWM,  NOPWM,  NOPWM,  M0PWM6, M0PWM7, NOPWM,  NOPWM,  // PC
    M1PWM0, M1PWM1, NOPWM,  NOPWM,  NOPWM,  NOPWM,  NOPWM,  NOPWM,  // PD
    NOPWM,  NOPWM,  NOPWM,  NOPWM,  M0PWM4, M0PWM5,                 // PE
    NOPWM,  NOPWM,  M1PWM6, M1PWM7, NOPWM                           // PF
};

volatile uint8_t pin_to_port[] = {
    PA, PA, PA, PA, PA, PA, PA, PA, // PA
    PB, PB, PB, PB, PB, PB, PB, PB, // PB
    PC, PC, PC, PC, PC, PC, PC, PC, // PC
    PD, PD, PD, PD, PD, PD, PD, PD, // PD
    PE, PE, PE, PE, PE, PE,         // PE
    PF, PF, PF, PF, PF              // PF
};

volatile uint8_t pin_to_port_bit[] = {
    0, 1, 2, 3, 4, 5, 6, 7, // PA
    0, 1, 2, 3, 4, 5, 6, 7, // PB
    0, 1, 2, 3, 4, 5, 6, 7, // PC
    0, 1, 2, 3, 4, 5, 6, 7, // PD
    0, 1, 2, 3, 4, 5,       // PE
    0, 1, 2, 3, 4           // PF
};

volatile uint8_t pin_to_pin_mask[] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, // PA
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, // PB
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, // PC
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, // PD
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20,             // PE
    0x01, 0x02, 0x04, 0x08, 0x10                    // PF
};

volatile uint8_t port_to_port_mask[] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20
};
