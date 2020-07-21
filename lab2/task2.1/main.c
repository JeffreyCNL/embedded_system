// task 2.1
// shine 7 colors for 1 Hz
// implemented by interrupt

#include "my_header.h"
#include <stdio.h>
#include <stdlib.h>

volatile uint32_t flags = 0;

// prototype
void portf_init(void);
void colorful(void);
void timer_config(void);
void gptm_polling(void);
void Timer_Handler(void);

int main()
{
  portf_init();
  GPIO_PORTF_DATA = 0x0;
  timer_config();
  
  flags = 0;
  while(1){
    colorful();
  } 
  return 0;
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
  // configure GPTMIMR register
  // TATOIM enable
  TIMER0_IMR |= 0x01;
  // enable interrupt
  // timer0 A is interrupt number 19
  NVIC_EN0 |= (1 << 19);  
  // enable the timer
  TIMER0_CTL |= 0x01;
}

void Timer_Handler(void){
  flags++;
  TIMER0_ICR |= 0x01;
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
  flags %= 7; 
  switch(flags){
  case 0:
    GPIO_PORTF_DATA = RED;
    break;
  case 1:
    GPIO_PORTF_DATA = BLUE;
    break;
  case 2:
    GPIO_PORTF_DATA = GREEN;
    break;
  case 3:
    GPIO_PORTF_DATA = (RED | BLUE);
    break;
  case 4:
    GPIO_PORTF_DATA = (RED | GREEN);
    break;
  case 5:
    GPIO_PORTF_DATA = (BLUE | GREEN);
    break;
  case 6:
    GPIO_PORTF_DATA = (RED | BLUE | GREEN);
    break;
  }
}