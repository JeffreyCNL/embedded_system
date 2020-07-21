// task 3
// FSM for taffic light using LCD to control
#include "my_header.h"
#include "tm4c123gh6pm.h"
#include "SSD2119.h"
#include "stdbool.h"

// prototype
typedef enum {OFF, STOP, WARN, GO}state;
void LCD_DrawCircle(unsigned short x0, unsigned short y0, unsigned short radius, short color);
void LCD_DrawFilledCircle(unsigned short x0, unsigned short y0, unsigned short radius, short color);
void LCD_DrawPixel(unsigned short x, unsigned short y, unsigned short color);
void LCD_DrawRect(unsigned short x, unsigned short y, short width, short height, unsigned short color);
void ui_define(void);
void Touch_BeginWaitForTouch(void);
void timer0_init(void);
void timer1_init(void);
void interrupt_init(void);
void set_state(state);
bool is_pressed(void);
void LCD_DisplayTouchPos(void);
void LCD_Calibration(void);

/*
unsigned short const Color4[16] = {
  0,                                            //0 ¡V black                   (#000000) 	000000 	0
 ((0x00>>3)<<11) | ((0x00>>2)<<5) | (0xAA>>3),  //1 ¡V blue                    (#0000AA) 	000001 	1
 ((0x00>>3)<<11) | ((0xAA>>2)<<5) | (0x00>>3),  //2 ¡V green                   (#00AA00) 	000010 	2
 ((0x00>>3)<<11) | ((0xAA>>2)<<5) | (0xAA>>3),  //3 ¡V cyan                    (#00AAAA) 	000011 	3
 ((0xAA>>3)<<11) | ((0x00>>2)<<5) | (0x00>>3),  //4 ¡V red                     (#AA0000) 	000100 	4
 ((0xAA>>3)<<11) | ((0x00>>2)<<5) | (0xAA>>3),  //5 ¡V magenta                 (#AA00AA) 	000101 	5
 ((0xAA>>3)<<11) | ((0x55>>2)<<5) | (0x00>>3),  //6 ¡V brown                   (#AA5500) 	010100 	20
 ((0xAA>>3)<<11) | ((0xAA>>2)<<5) | (0xAA>>3),  //7 ¡V white / light gray      (#AAAAAA) 	000111 	7
 ((0x55>>3)<<11) | ((0x55>>2)<<5) | (0x55>>3),  //8 ¡V dark gray /bright black (#555555) 	111000 	56
 ((0x55>>3)<<11) | ((0x55>>2)<<5) | (0xFF>>3),  //9 ¡V bright blue             (#5555FF) 	111001 	57
 ((0x55>>3)<<11) | ((0xFF>>2)<<5) | (0x55>>3),  //10 ¡V bright green           (#55FF55) 	111010 	58
 ((0x55>>3)<<11) | ((0xFF>>2)<<5) | (0xFF>>3),  //11 ¡V bright cyan            (#55FFFF) 	111011 	59
 ((0xFF>>3)<<11) | ((0x55>>2)<<5) | (0x55>>3),  //12 ¡V bright red             (#FF5555) 	111100 	60
 ((0xFF>>3)<<11) | ((0x55>>2)<<5) | (0xFF>>3),  //13 ¡V bright magenta         (#FF55FF) 	111101 	61
 ((0xFF>>3)<<11) | ((0xFF>>2)<<5) | (0x55>>3),  //14 ¡V bright yellow          (#FFFF55) 	111110 	62
 ((0xFF>>3)<<11) | ((0xFF>>2)<<5) | (0xFF>>3)   //15 ¡V bright white           (#FFFFFF) 	111111 	63
};
*/
#define LCDBLACK 0x0
#define LCDBLUE ((0x00>>3)<<11) | ((0x00>>2)<<5) | (0xAA>>3)
#define LCDGREEN ((0x00>>3)<<11) | ((0xAA>>2)<<5) | (0x00>>3)
#define LCDRED ((0xAA>>3)<<11) | ((0x00>>2)<<5) | (0x00>>3)

state system;
// set global variable for transling positoin
volatile int offset_X = 0;
volatile int offset_Y = 0;
volatile int xPos;
volatile int yPos;

int main()
{ 
  LCD_Init();
  Touch_Init(); // include ADC_Init();
  LCD_Calibration(); // help to stablize the value otherwise the value varies a lot
  ui_define();
  system = OFF;
  set_state(system);
  interrupt_init();
  timer0_init();
  timer1_init();

  while(1){
    // display the position when being touched
    LCD_DisplayTouchPos();
    is_pressed();
  }
  return 0;
}

bool is_pressed(void){
    if (Touch_ReadZ1() > 1500){       
      if (in_switch1(xPos, yPos) || in_switch2(xPos, yPos)){
            TIMER0_IMR |= 0x1;
            TIMER0_ICR |= 0x1;
            TIMER0_CTL |= 0x1;
      }
    }
}

void LCD_Calibration(void){
  LCD_DrawFilledCircle(160, 120, 10, 0x1234);
  LCD_SetCursor(60, 230);
  LCD_PrintString("Please Hit the Center Circle to Calibrate");

  int z = (int)Touch_ReadZ1();

  int x = (int)Touch_ReadX() / 10;
  int y = (int)Touch_ReadY() / 10;
  offset_X = abs(x - 160);
  offset_Y = abs(y - 120);
}

void LCD_DisplayTouchPos(void){
	int z = (int)Touch_ReadZ1();        
        xPos = (int)Touch_ReadX() / 10 + offset_X;
	yPos = (int)Touch_ReadY() / 10 - offset_Y;

	LCD_SetCursor(60, 230);
	printf("X Coor is: %d and Y Coor is: %d", xPos, yPos);
	LCD_SetCursor(60, 200);
	printf("Z1 Coor is: %d ", z);
}

void interrupt_init(void){
  NVIC_EN0 |= (1 << 21) | (1 << 19);  
}

 void timer1_init(void){
   // enable timer 0
   RCGCTIMER |= 37;
   // disable the timer. bit 0 GPTM Timer A enable
   TIMER1_CTL &= ~0x00000001;
   // select 32-bit mode 
   // 0x0 For a 16/32-bit timer, this value selects the 32-bit timer configuration
   TIMTER1_CONFIG = 0x0;
   // config TAMR bit to be in periodic timer mode
   // value 0x2 periodic timer mode
   TIMER1_TAMR = 0x2;
   // TACDIR bit to count down. 0 for count down
   TIMER1_TAMR |= 0x10;
   TIMER1_IMR |= 0x1;
   TIMER1_TAILR = 0x4C4B400;
   // this is where I stuck
   // need to clear out the value for potential previous 
   // left value
   TIMER1_ICR = 0x01;
 }

 void timer0_init(void){
   // enable timer 0
   RCGCTIMER |= 35;
   // disable the timer. bit 0 GPTM Timer A enable
   TIMER0_CTL &= ~0x00000001;
   // select 32-bit mode 
   // 0x0 For a 16/32-bit timer, this value selects the 32-bit timer configuration
   TIMTER0_CONFIG = 0x0;
   // config TAMR bit to be in periodic timer mode
   // value 0x2 periodic timer mode
   TIMER0_TAMR = 0x2;
   // TACDIR bit to count down. 0 for count down
   TIMER0_TAMR |= 0x10;
   
   TIMER0_IMR |= 0x1;
   TIMER0_TAILR = 0x1E84800;
   // this is where I stuck
   // need to clear out the value for potential previous 
   // left value
   TIMER0_ICR = 0x01;
 }

void set_state(state color){
  switch(color){
  case OFF:
    LCD_DrawFilledCircle(RED_XPOS, BULB_YPOS, RADIUS - 1, LCDBLACK);
    LCD_DrawFilledCircle(YELLOW_XPOS, BULB_YPOS, RADIUS - 1, LCDBLACK);
    LCD_DrawFilledCircle(GREEN_XPOS, BULB_YPOS, RADIUS - 1, LCDBLACK);
    break;
  case STOP:
    LCD_DrawFilledCircle(YELLOW_XPOS, BULB_YPOS, RADIUS - 1, LCDBLACK);
    LCD_DrawFilledCircle(GREEN_XPOS, BULB_YPOS, RADIUS - 1, LCDBLACK);
    LCD_DrawFilledCircle(RED_XPOS, BULB_YPOS, RADIUS - 1, LCDRED);
    break;
  case WARN:
    LCD_DrawFilledCircle(RED_XPOS, BULB_YPOS, RADIUS - 1, LCDBLACK);
    LCD_DrawFilledCircle(GREEN_XPOS, BULB_YPOS, RADIUS - 1, LCDBLACK);
    LCD_DrawFilledCircle(YELLOW_XPOS, BULB_YPOS, RADIUS - 1, LCDGREEN | LCDRED);
    break;
  case GO:
    LCD_DrawFilledCircle(RED_XPOS, BULB_YPOS, RADIUS - 1, LCDBLACK);
    LCD_DrawFilledCircle(YELLOW_XPOS, BULB_YPOS, RADIUS - 1, LCDBLACK);
    LCD_DrawFilledCircle(GREEN_XPOS, BULB_YPOS, RADIUS - 1, LCDGREEN);
    break;
  }
}


// check 2 sec
void Timer0_Handler(void){
  TIMER0_ICR_R |= 0x1; // clear timeout flag
  if (Touch_ReadZ1() > 1500){
    if (in_switch1(xPos, yPos)){
      if (system == OFF){
        TIMER1_ICR |= 0x01;
        TIMER1_CTL |= 0x01;
        system = STOP;
      }else{
        TIMER1_ICR |= 0x01;
        TIMER1_CTL &= ~0x01;
        system = OFF;
      }
    }    
    else if(in_switch2(xPos, yPos)){
      if (system == GO){
        TIMER1_ICR |= 0x01;
        system = WARN;
      }
    }
  }
  TIMER0_IMR &= ~0x1;
  TIMER0_ICR |= 0x01;
  TIMER0_CTL &= ~0x01;
  set_state(system);
}

// check 5 sec
void Timer1_Handler(void){
  TIMER1_ICR_R |= TIMER_ICR_TATOCINT; // clear timeout flag
  switch(system){
  case STOP:
    system = GO;
    break;
  case WARN:
    system = STOP;
    break;
  case GO:
    system = STOP;
    break;
  }
  set_state(system);
}

void ui_define(void){
  // pre layout three bulbs
  LCD_DrawCircle(RED_XPOS, BULB_YPOS, RADIUS, LCDRED);
  LCD_DrawCircle(YELLOW_XPOS, BULB_YPOS, RADIUS, LCDRED | LCDGREEN);
  LCD_DrawCircle(GREEN_XPOS, BULB_YPOS, RADIUS, LCDGREEN);
  
  // User Switches
  LCD_DrawFilledRect(SWITCH1_XPOS, SWITCH_YPOS, SWITCH_WIDTH, SWITCH_HEIGHT, LCDBLUE | LCDGREEN | LCDRED);
  LCD_DrawFilledRect(SWITCH2_XPOS, SWITCH_YPOS, SWITCH_WIDTH, SWITCH_HEIGHT, LCDBLUE | LCDGREEN | LCDRED);
  
  // print the string
  LCD_SetCursor(65, 165);
  LCD_PrintString("Start/Stop");
  LCD_SetCursor(195, 165);
  LCD_PrintString("Pedestrian");
}