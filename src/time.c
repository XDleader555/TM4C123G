#include "time.h"

// Page 710 of the datasheet states 80MHz clock (12.5 ns)
volatile unsigned long timer0_micros;
uint8_t timer_init;	// keep track of how the timer is initialized

void micros_handle(void) {
  timer0_micros ++;
}

void SysTick_Time_Init(void){
  if(timer_init == INIT_TIMER) {
    TIMER0_CTL_R &= 0xFFFFFFFE;   // 10) disable TIMER0A
  }
  timer_init = INIT_SYSTICK;

  timer0_micros = 0;								                // Clear our millis timer
  SysTick_Init_Custom(&(PLL_Init), &(micros_handle), 80, 0);  // Init the systick
}

// TODO: Add support for other timers and push the handler into its own file
void Timer_Time_Init(uint8_t timer) {
  if(timer_init == INIT_SYSTICK) {
    NVIC_ST_CTRL_R = 0x00;  // Disable Systick
  }
  timer_init = INIT_TIMER;

  timer0_micros = 0;
  Timer_Init_Custom(timer, &(micros_handle), 80);
}

unsigned long millis(void) {
  return timer0_micros / 1000;
}

unsigned long micros(void) {
  return timer0_micros;
}

void delay(unsigned long ms) {
  unsigned long startTime = millis();

  while(millis() - startTime <= ms) {} 
}

void delayMicros(uint16_t us) {
	unsigned long startTime;
  // return immediately if zero
  if(!us)
    return; 

  startTime = micros();
  
  // ToDo: Finish this function, figureout how long bitshifting takes
  while(micros() - startTime > us) {}
}
