#include <stdint.h>
#define RED 0x2   // PF1
#define BLUE 0x4  // PF2
#define GREEN 0x8 // PF3
#define PORTA_RED 0x04 // PA2
#define PORTA_YELLOW 0x08 // PA 3
#define PORTA_GREEN 0x10 // PA4
#define SWITCHES 0x11 // PF0 and PF4
#define GPIO_PORTF 0x20
#define GPIO_PORTA 0x01


// system control register
// according to data sheet "legacy software" the offset is 0x108 instead of 0x608
// base is 0x400F E000
#define RCGCGPIO (*((volatile uint32_t *) 0x400FE108))

// offset 0x400, base 0x4002 5000, p663
#define GPIO_PORTF_DIR (*((volatile uint32_t *) 0x40025400))

// stands for digital enable page 682
// base 0x4002 5000
// offset 0x51C
#define GPIO_PORTF_DEN (*((volatile uint32_t *) 0x4002551C))

// the base for GPIODIR register is 0x40025000
// in order for GPIODATA to write, bit [9:2] must be set
// therefore 0x3FC = 0b0011 1111 1100 is used
#define GPIO_PORTF_DATA (*((volatile uint32_t *) 0x400253FC))


// GPIOPUR pull up resistor
// base 0x4002.5000, offset 0x510
#define GPIO_PORTF_PUR (*((volatile uint32_t *) 0x40025510))


// GPIOCR commit
// base 0x4002.5000, offset 0x524
#define GPIOCR (*((volatile uint32_t *) 0x40025524))


// GPIOLOCK
// base 0x4002.5000, offset 0x520
#define GPIO_LOCK (*(( volatile uint32_t *)0x40025520))

// GPIOPCTL port control register
// base 0x4000.4000, offset 0x52C
#define GPIO_PORTA_PCTL (*((volatile uint32_t *) 0x4000452C))

// GPIO analog mode select p.687
// base 0x4000.4000, offset 0x528
#define GPIO_PORTA_AMSEL (*((volatile uint32_t *) 0x40004528))

// GPIO PORTA direction p663
// base 0x4000.4000, offset 0x400
#define GPIO_PORTA_DIR (*((volatile uint32_t *) 0x40004400))

// GPIO alternate function select p.672
// base 0x4000.4000, offset 0x420
// if set the associated pin functions as a peripheral signal and is
// control by the alternate hardware function
#define GPIO_PORTA_AFSEL (*((volatile uint32_t *)0x40004420))

// GPIO digital enable p.682
// base 0x4000.4000, offset 0x51C
#define GPIO_PORTA_DEN (*((volatile uint32_t *)0x4000451C))

// GPIO PORTA DATA p.662
// base 0x4002.5000, offset 
#define GPIO_PORTA_DATA (*((volatile uint32_t *)0x400043FC))

// base 0x400F.E000, offset 0x604
// p.338
#define RCGCTIMER (*((volatile uint32_t *) 0x400FE604))

// base 0x4003.0000, offset 0x00C
// p.737 timer0 control
#define TIMER0_CTL (*((volatile uint32_t *) 0x4003000C))
// base 0x4003.1000
#define TIMER1_CTL (*((volatile uint32_t *) 0x4003100C))

// base 0x4003.0000, offset 0x000
// p.727 GP timer config concatenated, individual, split
#define TIMTER0_CONFIG (*((volatile uint32_t *) 0x40030000))
// base 0x4003.1000
#define TIMTER1_CONFIG (*((volatile uint32_t *) 0x40031000))

// base 0x4003.0000, offset 0x004
// p.729 timer A mode
#define TIMER0_TAMR (*((volatile uint32_t *) 0x40030004))
// base 0x4003.1000
#define TIMER1_TAMR (*((volatile uint32_t *) 0x40031004))

// base 0x4003.0000, offset 0x028
// p.756 Timer A interval load
#define TIMER0_TAILR (*((volatile uint32_t *) 0x40030028))
// base 0x4003.1000
#define TIMER1_TAILR (*((volatile uint32_t *) 0x40031028))

// base 0x4003.0000, offset 0x01C
// p.748 raw interrupt status
#define TIMER0_RIS (*((volatile uint32_t *) 0x4003001C))
// base 0x4003.1000
#define TIMER1_RIS (*((volatile uint32_t *) 0x4003101C))

// base 0x4003.0000, offset 0x024
// p.754 interrupt clear
#define TIMER0_ICR (*((volatile uint32_t *) 0x40030024))
// base 0x4003.1000
#define TIMER1_ICR (*((volatile uint32_t *) 0x40031024))

// base 0x4003.0000, offset 0x018
// GPTM interrupt mask
#define TIMER0_IMR (*((volatile uint32_t *) 0x40030018))
// base 0x4003.1000, offset 0x018
#define TIMER1_IMR (*((volatile uint32_t *) 0x40031018))


// base 0xE000.E000, offset 0x100
// set enable
#define NVIC_EN0 (*((volatile uint32_t *) 0xE000E100))

// base 0x4002.5000, offset 0x418
// masked interrupt status
#define GPIO_PORTF_MIS (*((volatile uint32_t *) 0x40025418))
// base 0x4000.4000, offset 0x418
#define GPIO_PORTA_MIS (*((volatile uint32_t *) 0x40004418))


// base 0x4002.5000, offset 0x410
// interrupt mask
#define GPIO_PORTF_IM (*((volatile uint32_t *) 0x40025410))
// base 0x4000.4000, offset 0x410
#define GPIO_PORTA_IM (*((volatile uint32_t *) 0x40004410))

// base 0x4002.5000, offset 0x41C
// interrupt clear
#define GPIO_PORTF_ICR (*((volatile uint32_t *) 0x4002541C))
// base 0x4000.4000, offset 0x41C
#define GPIO_PORTA_ICR (*((volatile uint32_t *) 0x4000441C))

// base 0x4003.0000, offset 0x020
// timer interrupt status
#define TIMER0_MIS (*((volatile uint32_t *) 0x40030020))
// base 0x4003.1000
#define TIMER1_MIS (*((volatile uint32_t *) 0x40031020))

// base 0x400F.E000, offset 0x638
// adc control
#define RCGCADC (*((volatile uint32_t *) 0x400FE638))

// base 0x4003.8000, offset 0x000
// adc seqencer adcpssi ADCACTSS
#define ADC0_SEQ (*((volatile uint32_t *) 0x40038000))

// base 0x4003.8000, offset 0x014
// adc multiplexer adcemux
#define ADC0_MUX (*((volatile uint32_t *) 0x40038014))

// base  0x4003.8000, offset 0x0A0
// adc sample sequencer mux ADCSSMUX3
#define ADC0_SS_MUX3 (*((volatile uint32_t *) 0x400380A0))

// base  0x4003.8000, offset 0x0A4
// adc sample sequencer control ADCSSCTL3
#define ADC0_SS_CTL3 (*((volatile uint32_t *) 0x400380A4))


// base  0x4003.8000, offset 0x0A4
// adc sample sequencer control ADCSSCTL3
#define ADC0_SS_CTL3 (*((volatile uint32_t *) 0x400380A4))


// base  0x4003.8000, offset 0x028
// adc sample sequencer initiate ADCPSSI
#define ADC0_SS_INIT (*((volatile uint32_t *) 0x40038028))


// base  0x4003.8000, offset 0x004
// adc raw interrupt status ADCRIS
#define ADC0_RIS (*((volatile uint32_t *) 0x40038004))

// base  0x400F.E000, offset 0x070
// run-mode clock config
#define RCC2 (*((volatile uint32_t *) 0x400FE070))

// base  0x400F.E000, offset 0x060
// run-mode clock config
#define RCC (*((volatile uint32_t *) 0x400FE060))

// base  0x4003.8000, offset 0x00C
// interrupt status and clear ADCISC
#define ADC0_ISC (*((volatile uint32_t *) 0x4003800C))

// base  0x400F.E000, offset 0x050
// raw interrupt status
#define RIS (*((volatile uint32_t *) 0x400FE050))

// base  0x400F.E000, offset 0x058
// masked interrupt status and clear
#define MISC (*((volatile uint32_t *) 0x400FE058))

// base  0x4003.8000, offset 0x008
// adc interrupt mask
#define ADC0_IM (*((volatile uint32_t *) 0x40038008))

// base  0x4003.8000, offset 0x0A8
// sequencer result ADCSSFIFO3
#define ADC0_FIFO3 (*((volatile uint32_t *) 0x400380A8))

// base  0x400F.E000, offset 0x618
// UART clock gating
#define RCGCUART (*((volatile uint32_t *) 0x400FE618))

// base  0x4000.C000, offset 0x030
// UART control UARTCTL
#define UART0_CTL (*((volatile uint32_t *) 0x4000C030))

// base  0x4000.C000, offset 0x024
// UART int baud rate divisor UARTIBRD
#define UART0_IBRD (*((volatile uint32_t *) 0x4000C024))

// base  0x4000.C000, offset 0x028
// UART frac baud rate divisor UARTFBRD
#define UART0_FBRD (*((volatile uint32_t *) 0x4000C028))
// uart line control UARTLCRH
#define UART0_LCRH (*((volatile uint32_t *) 0x4000C02C))
// uart clock configuration UARTCC
#define UART0_CC (*((volatile uint32_t *) 0x4000CFC8))
// uart flag UARTFR
#define UART0_FR (*((volatile uint32_t *) 0x4000C018))
// uart data UARTDR
#define UART0_DR (*((volatile uint32_t *) 0x4000C000))


// UART register
#define UART_R0 0x1
#define UARTEN 0x1
#define WLEN_8BITS 0x30
#define FEN 0x10 // FIFO enable
#define STP2 0x40 // stop bit 2
#define PEN 0x20 // parity bit enable


// PLL register
#define RCC_BYPASS 0x800
#define RCC2_BYPASS2 0x800
#define RCC_USESYSDIV 0x400000
#define RCC_XTAL 0x7C0
#define RCC_XTAL_4MHZ 0x180
#define RCC_XTAL_16MHZ 0x540
#define RCC_OSCSRC 0x30
#define RCC_OSCSRC_MAIN 0x00
#define RCC2_OSCSRC2 0x70
#define RCC2_OSCSRC2_MAIN 0x00
#define RCC_PWRDN 0x2000
#define RCC2_PWRDN2 0x2000
#define RCC2_USERCC2 0x80000000
#define RCC2_DIV400 0x40000000

// adc registers
#define ASEN3 0x8
#define EMUX_EM3 0xF000
#define EMUX_TIMER 0x5000

// timer register
#define TAOTE 0x20
#define TAEN 0x1

#define Bus80MHz     4

// LCD UI define
#define SWITCH1_XPOS 55
#define SWITCH2_XPOS 185
#define SWITCH_YPOS 140
#define SWITCH_WIDTH 80
#define SWITCH_HEIGHT 60

#define DETECT_SWITCH1_XPOS_LEFT 280
#define DETECT_SWITCH1_XPOS_RIGHT 250
#define DETECT_SWITCH1_YPOS_UP 160
#define DETECT_SWITCH1_YPOS_DOWN 100

#define DETECT_SWITCH2_XPOS_LEFT 230
#define DETECT_SWITCH2_XPOS_RIGHT 180
#define DETECT_SWITCH2_YPOS_UP 130
#define DETECT_SWITCH2_YPOS_DOWN 100


#define  RED_XPOS 70
#define  YELLOW_XPOS 160
#define  GREEN_XPOS 250
#define RADIUS 40
#define  BULB_YPOS 80



