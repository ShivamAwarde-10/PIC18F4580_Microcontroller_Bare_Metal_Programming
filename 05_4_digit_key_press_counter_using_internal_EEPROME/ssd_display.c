#include <xc.h>
#include "ssd_display.h"

static unsigned char digit[] = {ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

void init_ssd_control(void)
{
    TRISD = 0x00;
    
    TRISA = TRISA & 0xF0;
    
    PORTA = PORTA & 0xF0;
}

void display(unsigned char data[])
{
    unsigned int wait;
    unsigned char digit_no;
    
    for(digit_no = 0; digit_no < 4; digit_no++)
    {
        PORTD = data[digit_no];
        
        PORTA = (PORTA & 0x0F) | (0x01 << digit_no);
        
        for(wait = 1000; wait--; );
    }
}

void display_count(unsigned int count)
{
    unsigned char ssd[4];
    
    ssd[0] = digit[(count % 10) / 1000];
    ssd[1] = digit[(count % 10) / 100];
    ssd[2] = digit[(count % 10) / 10];
    ssd[3] = digit[(count % 10)];
    
    display(ssd);
}
