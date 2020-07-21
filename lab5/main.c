#include <stdint.h>
//#include "tm4c123gh6pm.h"
#include "my_header.h"

void open_gate(uint16_t period);
void close_gate(void);
void wait(void);
void portf_init(void);
void user_switches(void);
void wait_n_sec(uint32_t sec);
void timer0_clear(void);
void timer0_init(void);
void pwm0_init(uint16_t period);
void pwm0_inversion_init(uint16_t period);
void pwm0_period(uint16_t period);
void GPIO_PortF_Handler(void);

uint16_t period = 1200; // set up the original period of the motor
int main(){
  int flag = 1;
  
  portf_init();
  user_switches();
  timer0_init();
  
  pwm0_init(period);
  while(1){
    
    // manually control the gate using user switch
    // hold the switch and the gate will keep open
//    while(!(GPIO_PORTF_DATA & (1))){
//        pwm0_period(period * 8 / 10);
//        pwm0_period(period);
//    }
    
    while(flag == 1){
      open_gate(period);
      break;
    }
    
    wait_n_sec(8);
    
  }
  return 0;
}

void GPIO_PortF_Handler(void){
  // SW2
  if (GPIO_PORTF_MIS & 1){
    // disable timer
    TIMER0_CTL &= ~0x01;
    // clear interrupt
    GPIO_PORTF_ICR = 0x01;
    pwm0_period(period * 8 / 10);
    pwm0_period(period); 
  }
}

// change period
// period is 16-bit number of PWM clock cycles in one period (3<=period)
void pwm0_period(uint16_t period){
  PWM0_LOAD = (period - 1)/2;       // 5) count from zero to this number and back to zero in (period - 1) cycles
  PWM0_CMPA = (period - 1)/4;       // 6) count value when PWM1/PB7 toggles
}


// reverse the polarity of the motor.
// will cause problem if keep reversing it
void pwm0_inversion_init(uint16_t period){
    PWM0_CTL = 0;                     // disable PWM while initializing
    PWM0_INVERT |= 0x1;
    //PWM0, Generator A (PWM0/PB6) goes to 0 when count==reload and 1 when count==0
    PWM0_GENA = (PWM0_GENA_ACTLOAD_ZERO | PWM0_GENA_ACTZERO_ONE);
    PWM0_LOAD = (period - 1)/2;       // count from zero to this number and back to zero in (period - 1) cycles
    PWM0_CMPA = (period - 1)/4;       // set up comparator A
    PWM0_CTL |= (MODE | ENABLE);      // set up count up/ down mode and enable the pwm0                                          
    PWM0_ENABLE |= PWM0EN;            // enable PWM0
}

void pwm0_init(uint16_t period){
    volatile uint32_t delay;
    RCGCPWM |= R0;                 // initialize PWM0
    RCGCGPIO |= GPIOB;             // initialize port B
    delay = RCGCGPIO;              // allow time to finish initilizing
    GPIO_PORTB_AFSEL |= (1 << 6);  // enable alt funct on PB6
    GPIO_PORTB_PCTL &= ~0xF000000;     // configure PB6 as PWM0
    GPIO_PORTB_PCTL |= 0x4000000;
    GPIO_PORTB_AMSEL &= ~(1 << 6);    // disable analog on PB6
    GPIO_PORTB_DEN |= GPIO_DEN_6;     // enable digital I/O on PB6
    RCC |= USEPWMDIV;                 // use PWM divider
    RCC &= ~RCC_PWMDIV;               // clear PWM divider field
    RCC += RCC_PWMDIV_64;             // configure for /64 divider
    PWM0_CTL = 0;                     // disable PWM while initializing
    //PWM0, Generator A (PWM0/PB6) goes to 0 when count==reload and 1 when count==0
    PWM0_GENA = (PWM0_GENA_ACTLOAD_ZERO | PWM0_GENA_ACTZERO_ONE);
    PWM0_LOAD = (period - 1)/2;       // count from zero to this number and back to zero in (period - 1) cycles
    PWM0_CMPA = (period - 1)/4;       // set up comparator A
    PWM0_CTL |= (MODE | ENABLE);      // set up count up/ down mode and enable the pwm0                                          
    PWM0_ENABLE |= PWM0EN;            // enable PWM0
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

void timer0_clear(void){ 
 TIMER0_CTL &= ~(0x01);
 TIMER0_ICR = 0x01;
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

void user_switches(void){
  GPIO_LOCK = 0x4C4F434B; // writing this to GPIOLOCK will unlock GPIOCR
  GPIOCR |= SWITCHES;
  GPIO_PORTF_PUR |= SWITCHES; // attach PF0 and PF4 to GPIOPUR
} 

void wait(void){
  for (int i = 0; i < 1000000; i++){}
}

void open_gate(uint16_t period){
  pwm0_period(period * 8 / 10);
  wait_n_sec(5);
  pwm0_period(period);
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
