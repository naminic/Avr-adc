/*****************************************************
Chip type               : ATmega32
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*****************************************************/

#include <mega32.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>      //use for call sprinf function in line 85. 
#include <stdlib.h>     //use for call ftoa function in line 84.

#define ADC_VREF_TYPE 0x20

unsigned char read_adc(unsigned char adc_input) //this function read signal from spaceficed pin in adc_input.
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);   //change channel.
delay_us(10);          //wait for change channel.

ADCSRA|=0x40;          //start the AD conversion.
while ((ADCSRA & 0x10)==0);    //wait for the AD conversion to complete.

ADCSRA|=0x10;          //clear interrupt flag.
return ADCH;
}

void main(void)
{
unsigned char init_result[7],lcd_result[10];
double bss;

ADMUX=ADC_VREF_TYPE & 0xff;    
ADCSRA=0x86;          //initiate AD for 125kHz clock, 8 bit result and AREF pin as voltage reference.

lcd_init(16);   //lcd connect to port D and 16 line mode.

while (1)
      {
        bss=read_adc(5);
        bss=bss*(3.3/256);
        
        lcd_clear();
        lcd_gotoxy(0,0);
        lcd_putsf("voltage is:");
        
        ftoa(bss,2,init_result);
        sprintf(lcd_result,"%4s volt.",init_result);
        
        lcd_gotoxy(0,1);
        lcd_puts(lcd_result);
        
        delay_ms(350);
      }
}
