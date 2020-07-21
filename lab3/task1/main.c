#include <stdlib.h>
#include <stdio.h>
#include "my_header.h"

// prototype
void timer0_init(void);
void portf_init(void);
void pll_init(void);
void adc_init(void);
void ADC0_Handler(void);
void color(double temp);
void uart_init(void);
void print_putty(double temp);
void check_press(void);

double temp = 0.0;
int main()
{
  portf_init();  
  pll_init();
  timer0_init();  
  adc_init();
 uart_init();
  while(1){
    check_press();
  }
  return 0;
}

void check_press(void){
    // check whether PF0 or PF4 is being pressed
    if ((GPIO_PORTF_DATA & 0x11) == 0x01) {
      // tuurn off the osccrc
      RCC2 &= ~RCC2_OSCSRC2;
      // use PIOSC/4 osc
      RCC2 |= 0x20;
      // turn on by pass
      RCC2 |= RCC2_BYPASS2;
      // turn off clock
      TIMER0_CTL &= ~(0x21);      
      // reset frequency
      TIMER0_TAILR = 0xF4240;// 4MHz
      // clear
      TIMER0_ICR |= 0x01;
      // turn on
      TIMER0_CTL |= 0x21;     
    }
    if ((GPIO_PORTF_DATA & 0x11) == 0x10) {
      // 
      RCC2 &= ~RCC2_OSCSRC2;
      RCC2 |= RCC2_OSCSRC2_MAIN;
      RCC2 |= RCC2_DIV400;   // use 400 MHz PLL 
      // start from SYSDIV LSB bit
      RCC2 = (4 << 22);
      // we want to use PLL so turn off the bypass
      RCC2 &= ~RCC2_BYPASS2;
      // turn off clock
      TIMER0_CTL &= ~(0x21);
      // reset value
      TIMER0_TAILR = 0x4C4B400; // 80MHz
      // clear
      TIMER0_ICR |= 0x01;
      // turn on
      TIMER0_CTL |= 0x21;
    }
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

void pll_init(void){
  //  use rcc2
  RCC2 |= RCC2_USERCC2;
  // bypass set before power dwn     
  RCC2 |= RCC2_BYPASS2;        
  // clear the XTAL before set up in case of 
  // previous different previous clock
  RCC &= ~(RCC_XTAL);
  RCC |= RCC_XTAL_16MHZ;

  RCC2 &= ~(RCC2_OSCSRC2);
  // select the main osc
  RCC2 |= RCC2_OSCSRC2_MAIN;
  // clear pwrdn to activate
  RCC2 &= ~(RCC2_PWRDN2);

  while(RIS & (0x40) == 0){}
  RCC2 &= ~RCC2_BYPASS2;
  }

void ADC0_Handler(void){
    // start new conversion ADCPSSI
  ADC0_SS_INIT |= (1 << 3);
  while (ADC0_RIS & (1 << 3) == 0){}
  temp = 147.5 - ((247.5 * ADC0_FIFO3) / 4096.0);
  // printf("%2.2f\n", temp);
  // clear the interrupt
  ADC0_ISC |= (1 << 3);
  print_putty(temp);
  color(temp);
}

void print_putty(double temp){
  char temp_arr[7];
  sprintf(temp_arr, "%lf", temp);
  temp_arr[5] = '\r';
  temp_arr[6] = '\n';
  for (int i = 0; i < 7; i++){
    while((UART0_FR & 0x20) != 0){}
    UART0_DR = temp_arr[i];
  }
}

void color(double temp){
  if (temp < 17){
    GPIO_PORTF_DATA = 1;
  }else if(temp < 19){
    GPIO_PORTF_DATA = 2;
  }else if (temp < 21){
    GPIO_PORTF_DATA = 3;
  }else if (temp < 23){
    GPIO_PORTF_DATA = 4;
  }else if (temp  < 25){
    GPIO_PORTF_DATA = 5;
  }else if (temp < 27){
    GPIO_PORTF_DATA = 6;
  }else{
    GPIO_PORTF_DATA = 7;
  }
}


void uart_init(void){
  RCGCUART |= UART_R0;
  RCGCGPIO |= GPIO_PORTA;
  //1. Disable the UART by clearing the UARTEN bit in the UARTCTL register.
  UART0_CTL &= ~UARTEN;
  //2. Write the integer portion of the BRD to the UARTIBRD register.
  UART0_IBRD |= 104; // 104
  // 3. Write the fractional portion of the BRD to the UARTFBRD register.
  UART0_FBRD |= 11;  
  // 4. Write the desired serial parameters to the UARTLCRH register (in this case, a value of
  // 0x0000.0060).
  UART0_LCRH |= 0x60; // no FIFO, 8 bits,  no parity, no interrupt
  // 5. Configure the UART clock source by writing to the UARTCC register.
  // UART0_CC |= 0x5; // use precision clock
  UART0_CC |= 0x0;
  // 7. Enable the UART by setting the UARTEN bit in the UARTCTL register
  UART0_CTL |= 0x301; // tx, rx uart0
  // 8. Make PA0 and PA1 pins to be used as digital I/O.
  GPIO_PORTA_DEN |= 0x3;
  // gpioamsel?
  // 9. Select the alternate functions of PA0 (RxD) and PA1 (TxD) pins using
  GPIO_PORTA_AFSEL |= 0x3;
  // 10. Configure PA0 and PA1 pins for UART function.
  GPIO_PORTA_PCTL |= 0x11; // pmc0 pmc 1    
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

void timer0_init(void){
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
  // init as 4MHZ timer
  TIMER0_TAILR = 4000000;
  
   TIMER0_ICR |= 0x1;
  
  // need to enable adc trigger
 TIMER0_CTL |= 0x21;
}
