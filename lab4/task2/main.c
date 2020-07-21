// task 2
// internal temperature sensor to print the current temperature on the LCD
#include "my_header.h"
#include "SSD2119.h"
#include "tm4c123gh6pm.h"

// prototype
void adc_init(void);
void ADC0_Handler(void);
void timer_init(void);

float temp;
int main()
{
  LCD_Init();
  timer_init();
  adc_init();
  while(1){
  }
  return 0;
}

void timer_init(void){
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
  TIMER0_CTL |= 0x21;
}

void adc_init(void){
  volatile unsigned long delay;
  // enable module 0
  RCGCADC |= 0x1;
  // a delay before sampling
  delay = RCGCADC;
  // disable sample sequencer 3 ADCACTSS
  ADC0_SEQ &= ~ASEN3;
  // select the trigger for timer
  ADC0_MUX &= ~EMUX_EM3;
  ADC0_MUX |= EMUX_TIMER;
  // select adc input channel ADCSSMUX3
  ADC0_SS_MUX3 &= 0x0;
  // one ended, raw interrupt enable, temp sensor read
  ADC0_SS_CTL3 |= 0xE;
        
        // clear the interrupt status for potential leftover
  ADC0_ISC |= (1 << 3);
  // interrupt #17
  NVIC_EN0 |= (1 << 17);
  // If interrupts are to be used, 
  // set the corresponding MASK bit in the ADCIM register
  ADC0_IM |= (1 << 3);
  // enable SS3
  ADC0_SEQ |= ASEN3;
}

void ADC0_Handler(void){
    // start new conversion ADCPSSI
  ADC0_SS_INIT |= (1 << 3);
  while (ADC0_RIS & (1 << 3) == 0){}
  temp = 147.5 - ((247.5 * ADC0_FIFO3) / 4096.0);
//   printf("%2.2f\n", temp);
  // clear the interrupt
  ADC0_ISC |= (1 << 3);
  LCD_PrintFloat(temp);
}
