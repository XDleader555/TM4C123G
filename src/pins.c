#include "pins.h"

// Port offsets
volatile unsigned long * ports[] = {
    GPIO_PORTA_DATA_BITS_R,
    GPIO_PORTB_DATA_BITS_R,
    GPIO_PORTC_DATA_BITS_R,
    GPIO_PORTD_DATA_BITS_R,
    GPIO_PORTE_DATA_BITS_R,
    GPIO_PORTF_DATA_BITS_R
};
