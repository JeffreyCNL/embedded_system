// task 2.3
 // traffic light system
// push start stop button more than two seconds to work
// wait 5 seconds for each state
// if ped button is pushed more than 2 second, go to warn state
// implemented by interrupt

 #include "my_header.h"
 #include <stdio.h>
 #include <stdlib.h>

 volatile uint32_t flag;
 // prototype
 void timer0_init(void);
 void timer1_init(void);
 void switch_init(void);
 void led_init(void);
 unsigned long start_stop_butt(void);
 unsigned long ped_butt(void);
 void traffic_light(uint32_t flag);
 void led_on(uint8_t color);
 void led_off(uint8_t color);
 void GPIO_PortA_Handler(void);
 void wait(void);
 void wait_test(void);

 int main()
 {
   led_init();
   switch_init();
   timer0_init(); // time 2 sec
   timer1_init(); // time 5 sec
   led_off(PORTA_RED);
   led_off(PORTA_GREEN);
   flag = 0;
   while(1){
     traffic_light(flag);
   }
   return 0;
 }

 void GPIO_PortA_Handler(void){
   // start stop is pressed
   while (GPIO_PORTA_MIS & (1 << 5)){
     // time out for timer 0, over 2 sec
     
     TIMER0_CTL |= 0x01;
     if (TIMER0_RIS & 1){
       TIMER0_ICR |= 0x01;
       GPIO_PORTA_DATA = 0x0;
       GPIO_PORTA_ICR |= 0x60;
       flag = 1;
       traffic_light(flag);
     }
   }
 }

 void traffic_light(uint32_t flag){
   switch (flag){
   case 0:
     led_off(PORTA_RED);
     led_off(PORTA_YELLOW);
     led_off(PORTA_GREEN);
     break;
   case 1:
     led_on(PORTA_RED);
     wait();
     led_off(PORTA_RED);
     flag = 2;
     traffic_light(flag);
     break;
   case 2:
     led_on(PORTA_GREEN);
     wait_test();
     led_off(PORTA_GREEN);
     flag = 1;
     traffic_light(flag);
     break;
   case 3:
     led_on(PORTA_YELLOW);
     wait();
     led_off(PORTA_YELLOW); 
     flag = 1;
     traffic_light(flag);
     break;
   }
 }

 void wait(void){
   TIMER1_CTL |= 0x01;
   while(TIMER1_RIS ^ 1){}
   TIMER1_ICR = 0x01;
   TIMER1_CTL &= ~0x01;
 }
 
 void wait_test(void){
   TIMER1_ICR |= 0x01;
   TIMER1_CTL |= 0x01;   
   TIMER0_ICR |= 0x01;
   TIMER0_CTL |= 0x01;
       
   while(TIMER1_RIS ^ 1){
//     while (GPIO_PORTA_MIS & (1 << 6)){
     while(ped_butt()){
       if (TIMER0_RIS & 1){
         GPIO_PORTA_ICR |= 0x60;
         TIMER0_ICR |= 0x01;
         TIMER1_CTL &= ~0x01;
         TIMER1_ICR |= 0x01;
         GPIO_PORTA_DATA = 0x0;
         flag = 3;
         traffic_light(flag);
       }
   }
   }
   TIMER1_ICR = 0x01;
   TIMER1_CTL &= ~0x01;
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
   TIMER0_TAILR = 0x1E84800;
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
   TIMER1_TAILR = 0x4C4B400;
   // this is where I stuck
   // need to clear out the value for potential previous 
   // left value
   TIMER1_ICR = 0x01;
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
   // interrupt number 0 for GPIO port A 
   NVIC_EN0 |= 0x01;
   GPIO_PORTA_IM |= 0x60;
 }

 unsigned long start_stop_butt(void){
   return (GPIO_PORTA_DATA & 0x20); // 0x20 (pressed) or 0 (not pressed)
 }

 unsigned long ped_butt(void){
   return (GPIO_PORTA_DATA & 0x40); // 0x40 (pressed) or 0 (not pressed)
 }

 void led_on(uint8_t color){
   GPIO_PORTA_DATA |= color;
 }

 void led_off(uint8_t color){
   GPIO_PORTA_DATA &= ~color;
 }