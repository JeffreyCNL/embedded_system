#include <stdlib.h>
#include <stdio.h>
#include "my_header.h"


// prototype
void timer0_init(void);
void portf_init(void);
void pll_init(void);
void adc_init(void);
void ADC0_Handler(void);
void color(uint16_t temp);
void read_temp(void);
void sys_clock_set(uint8_t flag);
void set_time(uint8_t flag);
void timeout_check(void);
void uart_init(void);
char readChar(void);
void printChar(char c);
void printString(char * string);

int main()
{
  portf_init();  
  uart_init();
  while(1){
    printString("r/g/b:\n\r");
    char c = readChar();
    printChar(c);
    printString("\n\r");
    switch(c){
    case 'r':
      GPIO_PORTF_DATA = RED;
      break;
    case 'b':
      GPIO_PORTF_DATA = BLUE;
      break;
    case 'g':
      GPIO_PORTF_DATA = GREEN;
      break;
    default:
      GPIO_PORTF_DATA = 0x0;
      break;
    }
  }
  return 0;
}

char readChar(void){
  char c;
  while ((UART0_FR & (1 << 4)) != 0);
  c = UART0_DR;
  return c;
}

void printChar(char c){
  while((UART0_FR & (1 << 5)) != 0);
  UART0_DR = c;
}

void printString(char * string){
  while(*string){
    printChar(*(string++));
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
// GPIOAFSEL
  GPIO_PORTA_AFSEL |= 0x3;
// 10. Configure PA0 and PA1 pins for UART function.
  GPIO_PORTA_PCTL |= 0x11; // pmc0 pmc 1
// 11. Wait for TxD output to establish idle high.
// 12. Monitor the TXFF flag bit in UART flag register and when it goes
// low (buffer not full), write a byte to data register to be transmitted
    
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
