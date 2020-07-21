// Task 1.2
// traffic light system
// push start stop button more than two seconds to work
// wait 5 seconds for each state
// if ped button is pushed more than 2 second, go to warn state

#include "my_header.h"
#include <stdio.h>
#include <stdlib.h>

// prototype
void timer0_init(void);
void timer1_init(void);
void wait_n_sec(uint32_t sec);
void wait_test(uint32_t sec);
void switch_init(void);
void led_init(void);
unsigned long start_stop_butt(void);
unsigned long ped_butt(void);
void traffic_light(void);
void led_on(uint8_t color);
void led_off(uint8_t color);
uint8_t check_push_time(uint32_t sec);
void timer0_clear(void);
void timer0_reset(uint32_t sec);
void timer1_clear(void);
void timer1_reset(uint32_t sec);

int main()
{
  led_init();
  switch_init();
  timer0_init();
  timer1_init();
  led_off(PORTA_RED);
  led_off(PORTA_YELLOW);
  led_off(PORTA_GREEN);
  while(!check_push_time(2)){}
  while(1){
    traffic_light();
  }
  return 0;
}

void traffic_light(void){
    while(ped_butt()){
      led_on(PORTA_RED);
    }  
    // pedestrian not pressed, switch between go and stop states
    led_on(PORTA_RED);
    wait_n_sec(5);
    led_off(PORTA_RED);
    led_on(PORTA_GREEN);
    
    wait_test(5);
    led_off(PORTA_GREEN);
    traffic_light();
}

void timer0_init(void){
  // enable timer 0
  RCGCTIMER = 0x03;
  // disable the timer. bit 0 GPTM Timer A enable
  TIMER0_CTL &= 0x00000000;
  // select 32-bit mode 
  // 0x0 For a 16/32-bit timer, this value selects the 32-bit timer configuration
  TIMTER0_CONFIG = 0x0;
  // config TAMR bit to be in periodic timer mode
  // value 0x2 periodic timer mode
  TIMER0_TAMR = 0x2;
  // TACDIR bit to count down. 0 for count down
  TIMER0_TAMR &= ~0x10;
  // this is where I stuck
  // need to clear out the value for potential previous 
  // left value
  TIMER0_ICR = 0x01;
}

void timer1_init(void){
  // enable timer 0
  RCGCTIMER = 0x03;
  // disable the timer. bit 0 GPTM Timer A enable
  TIMER1_CTL &= 0x00000000;
  // select 32-bit mode 
  // 0x0 For a 16/32-bit timer, this value selects the 32-bit timer configuration
  TIMTER1_CONFIG = 0x0;
  // config TAMR bit to be in periodic timer mode
  // value 0x2 periodic timer mode
  TIMER1_TAMR = 0x2;
  // TACDIR bit to count down. 0 for count down
  TIMER1_TAMR &= ~0x10;
  // this is where I stuck
  // need to clear out the value for potential previous 
  // left value
  TIMER1_ICR = 0x01;
}

// disable timer
// and set clear the TATORIS bit
void timer0_clear(void){ 
 TIMER0_CTL &= ~(0x01);
 TIMER0_ICR = 0x01;
}

void timer1_clear(void){
  TIMER1_CTL &= ~(0x01);
  TIMER1_ICR = 0x01;
}

void timer0_reset(uint32_t sec){
    TIMER0_TAILR = 16000000 * sec;
    TIMER0_CTL |= 0x01;
}

void timer1_reset(uint32_t sec){
    TIMER1_TAILR = 16000000 * sec;
    TIMER1_CTL |= 0x01;
}

void wait_n_sec(uint32_t sec){
  timer0_clear();
  // value 16,000,000
  // learn how to config this value
  TIMER0_TAILR = 16000000 * sec;
  // enable the timer
  TIMER0_CTL |= 0x01;
  // poll TATORIS bit 0
  // 0 has not time out
  // 1 has time out
  while(TIMER0_RIS ^ 0x01){}    
 // clear bit TATOCINT. writing 1 to bit 0
//  TIMER0_ICR = 0x01;
  timer0_clear();
}

void wait_test(uint32_t sec){ 
  // poll TATORIS bit 0
  // 0 has not time out
  // 1 has time out
  timer1_clear();
  timer1_reset(2);
  timer0_clear();
  timer0_reset(sec);
  while(TIMER0_RIS ^ 0x01){
    while (ped_butt()){
      if (TIMER1_RIS == 0x01){
        led_off(PORTA_GREEN);
        led_on(PORTA_YELLOW);
        wait_n_sec(5);
        led_off(PORTA_YELLOW);
        timer1_clear();
        traffic_light();
      }
    }
  }    
  timer0_clear();
}

uint8_t check_push_time(uint32_t sec){
  timer0_clear();
  timer0_reset(sec);
  while (start_stop_butt()){
    if (TIMER0_RIS == 0x1){      
      timer0_clear();
      return 1;
    }
  }
  return 0;
}

void led_init(void){
  volatile unsigned long delay;
  RCGCGPIO |= GPIO_PORTA;      // activate clock for port A
  delay = RCGCGPIO;            // allow time for clock to start
  GPIO_PORTA_PCTL &= ~0x000FFF00; // regular GPIO pin2 3 4
  GPIO_PORTA_AMSEL &= ~0x1C;      // disable analog functions of PA2, PA3, PA4
  GPIO_PORTA_DIR |= 0x1C;         // set PA2 to 4 to output
  GPIO_PORTA_AFSEL &= ~0x1C;      // regular port function
  GPIO_PORTA_DEN |= 0x1C;         // enable digital output on PA2 to 4
}

void switch_init(void){
  volatile unsigned long delay;
  RCGCGPIO |= GPIO_PORTA;  // activate clock for Port A
  delay = RCGCGPIO; // allow time for clock to start
  GPIO_PORTA_AMSEL &= ~0x60; // disable analog on PA5 PA6
  GPIO_PORTA_PCTL &= ~0x0FF00000; // PCTL GPIO on PA5 PA6
  GPIO_PORTA_DIR &= ~0x60; // set PA5 PA6 to input
  GPIO_PORTA_AFSEL &= ~0x60; // PA5 PA6 regular port function
  GPIO_PORTA_DEN |= 0x60; // enable PA5 PA6 as digital port
}

unsigned long start_stop_butt(void){
  return (GPIO_PORTA_DATA & 0x20); // 0x20 (pressed) or 0 (not pressed)
}

unsigned long ped_butt(void){
  // printf("%li", GPIO_PORTA_DATA & 0x40);
  return (GPIO_PORTA_DATA & 0x40); // 0x40 (pressed) or 0 (not pressed)
}

void led_on(uint8_t color){
  GPIO_PORTA_DATA |= color;
}

void led_off(uint8_t color){
  GPIO_PORTA_DATA &= ~color;
}
