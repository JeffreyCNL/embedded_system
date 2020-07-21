// Task 1.1
// shine 7 colors for 1 Hz

#include "my_header.h"
#include <stdio.h>
#include <stdlib.h>

// prototype
void portf_init(void);
void colorful(void);
void timer_config(void);
void gptm_polling(void);

int main()
{
  portf_init();
  timer_config();
  while(1){
    colorful();
  }
  return 0;
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

// turn on 7 color combination periodically
void colorful(void){
  GPIO_PORTF_DATA = RED;
  gptm_polling();
  GPIO_PORTF_DATA = BLUE;
  gptm_polling();
  GPIO_PORTF_DATA = GREEN;
  gptm_polling();
  GPIO_PORTF_DATA = (RED | BLUE);
  gptm_polling();
  GPIO_PORTF_DATA = (RED | GREEN);
  gptm_polling();
  GPIO_PORTF_DATA = (BLUE | GREEN);
  gptm_polling();
  GPIO_PORTF_DATA = (RED | BLUE | GREEN);
  gptm_polling();
}

void timer_config(void){
  // enable timer 0
  RCGCTIMER = 0x01;
  // disable the timer. bit 0 GPTM Timer A enable
  TIMER0_CTL &= 0x0;
  // select 32-bit mode 
  // 0x0 For a 16/32-bit timer, this value selects the 32-bit timer configuration
  TIMTER0_CONFIG = 0x0;
  // config TAMR bit to be in periodic timer mode
  // value 0x2 periodic timer mode
  TIMER0_TAMR = 0x2;
  // TACDIR bit to count down. 0 for count down
  TIMER0_TAMR &= ~0x10;
  // value 16,000,000
  TIMER0_TAILR = 0xF42400; 
  // enable the timer
  TIMER0_CTL |= 0x01;
}

void gptm_polling(void){
  // poll TATORIS bit 0
  // 0 has not time out
  // 1 has time out
  while(TIMER0_RIS ^ 0x01){}
  // clear bit TATOCINT. writing 1 to bit 0
  TIMER0_ICR = 0x01;
}

