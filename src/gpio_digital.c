#include "gpio_digital.h"

static volatile unsigned long clockDelay;

static GPIOPort_ISR GPIOPortA_ISR[8];
static GPIOPort_ISR GPIOPortB_ISR[8];
static GPIOPort_ISR GPIOPortC_ISR[8];
static GPIOPort_ISR GPIOPortD_ISR[8];
static GPIOPort_ISR GPIOPortE_ISR[6];
static GPIOPort_ISR GPIOPortF_ISR[5];

static GPIOPort_ISR *GPIOPortISR_List[] = {
    GPIOPortA_ISR,
    GPIOPortB_ISR,
    GPIOPortC_ISR,
    GPIOPortD_ISR,
    GPIOPortE_ISR,
    GPIOPortF_ISR
};

static uint8_t GPIOPort_INT_NUM[] = {
  0,  // GPIO Port A
  1,  // GPIO Port B
  2,  // GPIO Port C
  3,  // GPIO Port D
  4,  // GPIO Port E
  30  // GPIO Port F
};

static volatile unsigned long * GPIOPort_INT_PRI[] = {
    &NVIC_PRI0_R,
    &NVIC_PRI0_R,
    &NVIC_PRI0_R,
    &NVIC_PRI0_R,
    &NVIC_PRI1_R,
    &NVIC_PRI7_R
};

static uint8_t GPIOPort_INT_PRI_OFFSET[] = {
    5,
    13,
    21,
    29,
    5,
    21
};

void pinMode(uint8_t pin, uint8_t mode) {
  volatile unsigned long * port = port_addrs[pin_to_port[pin]];
	uint8_t port_mask = _8bit_mask[pin_to_port[pin]];
  uint8_t pin_mask = pin_to_pin_mask[pin];

  SYSCTL_RCGC2_R |= ((uint64_t) port_mask);   // gpio clock
  clockDelay = SYSCTL_RCGC2_R;                // delay by assigning a register
    portData(port, P_LOCK)   =  0x4C4F434B;     // unlock port
  setbit(portData(port, P_CR), pin_mask);     // allow changes to pin
  clrbit(portData(port, P_AFSEL), pin_mask);  // Regular IO
	clrbit(portData(port, P_AMSEL), pin_mask);  // Force digital instead of analog
  portData(port, P_PCTL) &= (uint32_t) ~(0xF << (pin_to_port_bit[pin] * 4)); // clear the pin mux control
  
  clrbit(portData(port, P_PUR), pin_mask);    // clear pullup bit
  clrbit(portData(port, P_PDR), pin_mask);    // clear pulldown bit

  if(mode == OUTPUT) {
    setbit(portData(port, P_DIR), pin_mask);  // Set Direction as output
  } else if(mode == INPUT) {
		clrbit(portData(port, P_DIR), pin_mask);  // Set Direction as input
	} else if(mode == INPUT_PULLUP) {
		clrbit(portData(port, P_DIR), pin_mask);  // Set Direction as input
		setbit(portData(port, P_PUR), pin_mask);  // enable pullup resistor
	} else if(mode == INPUT_PULLDOWN) {
		clrbit(portData(port, P_DIR), pin_mask);  // Set Direction as input
		setbit(portData(port, P_PDR), pin_mask);	// enable pulldown resistor
	}
  
  setbit(portData(port, P_DEN), pin_mask);    // enable digital pins
}

void attachInterrupt(uint8_t pin, void (*ISR)(void), uint8_t mode, uint32_t priority) {
  volatile unsigned long * port = port_addrs[pin_to_port[pin]];
  uint8_t pin_mask = pin_to_pin_mask[pin];

  clrbit(portData(port, P_IM), pin_mask);  // Disable Interrupts during setup
  
  (GPIOPortISR_List[pin_to_port[pin]])[pin_to_port_bit[pin]] = ISR; // Assign the ISR

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

  //setbit(portData(port, P_RIS), pin_mask);  // Reset Raw Interrupt Status Flag
  setbit(portData(port, P_ICR), pin_mask);  // Reset Interrupt Flag
  setbit(portData(port, P_IM), pin_mask);  // Enable Interrupt

  // Find NVIC PRI from Page 152
	(*(GPIOPort_INT_PRI[pin_to_port[pin]])) &= (unsigned long) ~(0x00000007 << GPIOPort_INT_PRI_OFFSET[pin_to_port[pin]]); // clear priority
  (*(GPIOPort_INT_PRI[pin_to_port[pin]])) |= ((priority & 0x00000007) << GPIOPort_INT_PRI_OFFSET[pin_to_port[pin]]); // write priority

  // Find interrupt Number from Page 104
  NVIC_EN0_R |= (0x01 << GPIOPort_INT_NUM[pin_to_port[pin]]); // Enable the interrupt
}

// Set a pin to low or high
void digitalWrite(uint8_t pin, uint8_t data) {
  volatile unsigned long * port = port_addrs[pin_to_port[pin]];
  uint8_t pin_mask = pin_to_pin_mask[pin];
	
	data = data & 0x01; // mask data for data inversions

  if(data) {
    setbit(portData(port, P_DATA), pin_mask);
  } else {
    clrbit(portData(port, P_DATA), pin_mask);
  }
}

uint8_t digitalRead(uint8_t pin) {
  volatile unsigned long * port = port_addrs[pin_to_port[pin]];
  uint8_t pin_mask = pin_to_pin_mask[pin];

  if(portData(port, P_DATA) & pin_mask) {
    return HIGH;
  }
  
  return LOW;
}

void GPIOPort_Handler(uint8_t portnum, uint8_t port_addrsize) {
  volatile unsigned long * port = port_addrs[portnum];
  uint8_t isr;

  for(isr = 0; isr < port_addrsize; isr ++)
    if(((portData(port, P_MIS) >> isr) & 0x1) == 1)
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
