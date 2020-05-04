#include "timer.h"

void (*_Timer0A_Handler)(void);

void Timer_Init_Custom(uint8_t timer, void (*handler)(void), uint32_t reload) {
  uint32_t clockDelay;

  // Assign internal handler for when the timer overflows
  _Timer0A_Handler = handler;

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
  (*_Timer0A_Handler)();
}