#include <xc.h>
#include "ssd_display.h"

void init_ssd_control(void)
{
    TRISD = 0x00;                 // Configure PORTD as outputs for SSD segment data
    
    TRISA = TRISA & 0xF0;         // Configure RA0?RA3 as outputs
    
    PORTA = PORTA & 0xF0;         // Clear RA0?RA3 (disable all SSD digits initially)
}

void display(unsigned char data[])
{
    unsigned int wait;
    unsigned char digit;
    
    for(digit = 0; digit < 4; digit++)
    {
        PORTD = data[digit];
        
        PORTA = (PORTA & 0xF0) | (1 << digit);
        
        for(wait = 1000; wait--;);
    }
}
