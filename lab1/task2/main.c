/*
  this is for controlling traffic light
  signal by switches
  pressing start and stop button will initialize FSM
  pressing pedestrian button will go to warn and stop state

*/

/*
  I started all this project from scratch
  Reading datasheet and registers trying to bit manipulate them all
  Though it's short and simple. This is the first time
  I can dig down deep into function instead of API call-out
  That's why I am proud of this.
*/

#include <stdint.h>
#include <stdlib.h>
#include "my_header.h"

// prototype
void wait(void); 
void led_init(void);
void red_on(void);
void red_off(void);
void yellow_on(void);
void yellow_off(void);
void green_on(void);
void green_off(void);
void switch_init(void);
void traffic_light(void);
unsigned long start_stop_butt(void);
unsigned long ped_butt(void);

int main()
{
  led_init();
  switch_init();
  // turn off all before start
  red_off();
  green_off();
  do{
    
  }while(!start_stop_butt());
  while (1){
    traffic_light();
  }
  return EXIT_SUCCESS;
}

// simply wait about 0.3 seconds
void wait(void){
  int j;
  for (j = 0; j < 1000000; j++){}
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

// PA2
void red_on(void){
  GPIO_PORTA_DATA |= 0x04;
}

void red_off(void){
  GPIO_PORTA_DATA &= ~0x04;
}

// PA3
void yellow_on(void){
  GPIO_PORTA_DATA |= 0x08;
}

void yellow_off(void){
  GPIO_PORTA_DATA &= ~0x08;
}

// PA4
void green_on(void){
  GPIO_PORTA_DATA |= 0x10;
}
  
void green_off(void){
  GPIO_PORTA_DATA &= ~0x10;
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
  return (GPIO_PORTA_DATA & 0x40); // 0x40 (pressed) or 0 (not pressed)
}

void traffic_light(void){  
    // pedestrian not pressed, switch between go and stop states
    red_on();
    wait();
    red_off();
    green_on();
    wait();    
    // ped button will check only when green (go state) is on
    if(ped_butt()){
      wait();
      green_off();
      wait();
      yellow_on();
      wait();
      yellow_off();
      // if keep pushing, red keep on
      while(ped_butt()){
        red_on();
      }
    }
    green_off();
}
