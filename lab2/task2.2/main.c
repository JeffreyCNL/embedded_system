// task 2.2
// interrupt when the user switch is pushed
// the system will blink blue led in 1 Hz
// when user switch 2 is pressed, stop the timer and go into red led
// press switch 1 to resume the system


#include <stdint.h>
#include <stdlib.h>
#include "my_header.h"

// prototype
void portf_init(void);
void colorful(void);
void user_switches(void);
void timer_config(void);
void blue_blink(void);
void timer_reset(void);
void GPIO_PortF_Handler(void);

volatile uint32_t flag;

int main()
{
  portf_init();
  GPIO_PORTF_DATA = 0x0; // turn off light before start
  user_switches();
  timer_config();

  while (1){
      blue_blink();
  }
  return EXIT_SUCCESS;
}

void GPIO_PortF_Handler(void){
  // SW2
  if (GPIO_PORTF_MIS & 1){
    // disable timer
    TIMER0_CTL &= ~0x01;
    // clear interrupt
    GPIO_PORTF_ICR = 0x01;
    GPIO_PORTF_DATA = RED;
  }
  // SW1
  if (GPIO_PORTF_MIS & (1 << 4)){
    // clear the interrupt
     GPIO_PORTF_ICR = 0x10;
     blue_blink();
  }
}

void timer_reset(void){
  TIMER0_ICR = 0x01;
  TIMER0_CTL |= 0x01;
}

void blue_blink(void){    
    GPIO_PORTF_DATA = BLUE;
    timer_reset();
    while(TIMER0_RIS ^ 0x01){}
    // clear bit TATOCINT. writing 1 to bit 0
    GPIO_PORTF_DATA = 0x0;
    timer_reset();
    while(TIMER0_RIS ^ 0x01){}
}

void portf_init(void){
  RCGCGPIO = GPIO_PORTF; // enable port F GPIO
  // turn on PF1, PF2, PF3 all at once
  // will waste more energy, but the code will be a lot cleaner
  // DIR for PF1 to 3 as output set. clear 1 and 4 to be input for user switches
  GPIO_PORTF_DIR = 0xE;
  // DEN enable all PF0 to PF4 port for RGB and switches
  GPIO_PORTF_DEN = 0x1F;

}

void timer_config(void){
  // enable timer 0
  RCGCTIMER = 0x01;
  // disable the timer. bit 0 GPTM Timer A enable
  TIMER0_CTL &= 0x0;
  // select 32-bit mode 
  // 0x0 For a 16/32-bit timer, this value selects the 32-bit timer configuration
  TIMTER0_CONFIG = 0x00000000;
  // config TAMR bit to be in periodic timer mode
  // value 0x2 periodic timer mode
  TIMER0_TAMR = 0x2;
  // TACDIR bit to count down. 0 for count down
  TIMER0_TAMR &= ~0x10;
  // value 16,000,000
  TIMER0_TAILR = 0xF42400;
}

void user_switches(void){
  GPIO_LOCK = 0x4C4F434B; // writing this to GPIOLOCK will unlock GPIOCR
  GPIOCR |= SWITCHES;
  GPIO_PORTF_PUR |= SWITCHES; // attach PF0 and PF4 to GPIOPUR
  // enable interrupt
  // GPIO port F is interrupt number 30
  NVIC_EN0 |= (1 << 30);
  // enable PF4, PF0 interrupt mask
  GPIO_PORTF_IM = 0x11;
} 
