#include "pwm.h"
#include "gpio_digital.h"

// Todo: technically the general purpose timers can be used for pwm on pins
// which dont have dedicated pwm hardware
void analogWrite(uint8_t pin, uint16_t value) {
  volatile unsigned long * port = port_addrs[pin_to_port[pin]];
  volatile unsigned long * pwmcfgaddr;
  uint8_t pin_mask = pin_to_pin_mask[pin];
  uint8_t pctl_shift = pin_to_port_bit[pin] * 4;
  uint8_t pwm = pin_to_pwm[pin];
  uint8_t pwm_ena = pwm_to_enable_bit[pwm];
  uint8_t pwm_gen = pwm_to_pwmgen[pwm];
  uint8_t pwm_mod = pwm_to_pwmmod(pwm_gen);
  uint32_t pwm_gen_mask = 0x00000001 << pwm_gen;
  value = max(min(value, PWM_RES), 0);   // Max value is 12-bit to match the ADC

  // Get the offset for the pwmcfg
  pwmcfgaddr = portDataAddr(pwm_to_pwmmodaddr(pwm), PWM_CFGSIZE * pwm_gen);

  // setup gpio as output if it's not already
  if(!(portData(port, P_DIR) & pin_mask)) {
    pinMode(pin, OUTPUT);
  }

  // If there's no PWM on this pin, write a digital value and return
  if(pwm_gen == NOPWM) {
    if(value < PWM_RES/2) {
      digitalWrite(pin, LOW);
    } else {
      digitalWrite(pin, HIGH);
    }
    return;
  }

  // Full on and off conditions
  // if(value == PWM_RES) {
  //   // Full on, reconfigure as GPIO 
  //   clrbit(portData(port, P_AFSEL), pin_mask);  // Regular IO
  //   portData(port, P_PCTL) &= (uint32_t) ~(0xF << (pin_to_port_bit[pin] * 4)); // clear the pin mux control
  //   digitalWrite(pin, HIGH);
  // } else if (value == 0) {
  // if(pwm_mod == 0) {
  //   PWM0_ENABLE_R &= ~(_8bit_mask[pwm_ena]);
  // } else {
  //   PWM1_ENABLE_R |= ~(_8bit_mask[pwm_ena]);
  // }
  // }

  // setup gpio as pwm if it's not already (Check if the PWM bits have been set)
  // Stuff in here only needs to be set first time PWM is setup for this pin
  if(((portData(port, P_PCTL) >> pctl_shift) & 0x07) < 4) {
    //digitalWrite(pin, LOW); // Clear digital output
    setbit(portData(port, P_AFSEL), pin_mask);      // alt function
    if(pwm_mod == 0) {
      portData(port, P_PCTL) |= (uint32_t) (0x4 << pctl_shift);  // write the mux control for PWM0
    } else {
      portData(port, P_PCTL) |= (uint32_t) (0x5 << pctl_shift);  // write the mux control for PWM1
    }

    SYSCTL_RCGCPWM_R |= pwm_mod ? 0x01 : 0x02;  // activate the appropriate pwm module
    SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV;       // use PWM divider
    SYSCTL_RCC_R &= (uint32_t) ~SYSCTL_RCC_PWMDIV_M;       // clear PWM divider field
    SYSCTL_RCC_R |= SYSCTL_RCC_PWMDIV_2;        // configure for /2 divider
    portData(pwmcfgaddr, PWM_CTL) = 0; // disable the appropriate pwm generator for setup
    // low on LOAD, high on CMPA down, even enable bits use generator A
    if(pwm_ena % 2 == 0) {
      portData(pwmcfgaddr, PWM_GENA) = (PWM_0_GENB_ACTCMPBD_ONE|PWM_0_GENB_ACTLOAD_ZERO);
    } else {
      portData(pwmcfgaddr, PWM_GENB) = (PWM_0_GENB_ACTCMPBD_ONE|PWM_0_GENB_ACTLOAD_ZERO);
    }
    portData(pwmcfgaddr, PWM_LOAD) = PWM_PERIOD - 1;           // 5) cycles needed to count down to 0
    portData(pwmcfgaddr, PWM_CTL) |= pwm_gen_mask;   // start the appropriate pwm timer
    // Enable PWM
    if(pwm_mod == 0) {
      PWM0_ENABLE_R |= (uint32_t) _8bit_mask[pwm_ena];
    } else {
      PWM1_ENABLE_R |= (uint32_t) _8bit_mask[pwm_ena];
    }
  } else {
    // Just need to reload the value
    portData(pwmcfgaddr, PWM_CMPA) = ((uint16_t) (PWM_PERIOD * (((double) value)/PWM_RES))) - 1;  // 6) count value when output rises
  }
}

void analogWriteFrequency(uint16_t freq) {
  // todo is this correct?
  PWM_PERIOD = (uint32_t) ((1.0/freq)/(0.0000000125));
}
