/*
	this is a project that can shine 7 colors on the board
	By pressing the two user switches on the board, we can stall in color red and blue
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
void portf_init(void);
void colorful(void);
void user_switches(void);

// simply wait about 0.3 seconds
void wait(void){
  int j;
  for (j = 0; j < 1000000; j++){}
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
  wait();
  GPIO_PORTF_DATA = BLUE;
  wait();
  GPIO_PORTF_DATA = GREEN;
  wait();
  GPIO_PORTF_DATA = (RED | BLUE);
  wait();
  GPIO_PORTF_DATA = (RED | GREEN);
  wait();
  GPIO_PORTF_DATA = (BLUE | GREEN);
  wait();
  GPIO_PORTF_DATA = (RED | BLUE | GREEN);
  wait();
}

void user_switches(void){
  GPIO_LOCK = 0x4C4F434B; // writing this to GPIOLOCK will unlock GPIOCR
  GPIOCR |= SWITCHES;
  GPIO_PORTF_PUR |= SWITCHES; // attach PF0 and PF4 to GPIOPUR
} 

int main()
{
  portf_init();
  GPIO_PORTF_DATA = 0x0; // turn off light before start
  user_switches();

  while (1){
      colorful();

//   this function will wait the colorful func to end
//   not an interrupt func
//   when switch 2 is pressed, set green
    while(!(GPIO_PORTF_DATA & (1))){
       GPIO_PORTF_DATA = GREEN;
    }
    // when switch 1 is pressed, set red
    while(!(GPIO_PORTF_DATA & (1 << 4))){
       GPIO_PORTF_DATA = RED;
    }
  }
  return EXIT_SUCCESS;
}




