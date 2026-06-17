#include <xc.h>
#include "ssd.h"

static unsigned char digit[] = { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE };

void init_ssd_control(void)
{
    TRISD = 0x00;          // SSD data lines output

    TRISA = TRISA & 0xF0; // RA0-RA3 output

    PORTA = PORTA & 0xF0; // Disable all SSDs
}

void display(unsigned char data[])
{
    unsigned int wait;
    unsigned char digit_no;

    for(digit_no = 0; digit_no < 4; digit_no++)
    {
        PORTD = data[digit_no];

        PORTA = (PORTA & 0xF0) | (0x01 << digit_no);

        for(wait = 1000; wait--; );
    }
}

void display_count(unsigned int count)
{
    unsigned char ssd[4];

    ssd[0] = digit[(count / 1000) % 10];
    ssd[1] = digit[(count / 100) % 10];
    ssd[2] = digit[(count / 10) % 10];
    ssd[3] = digit[count % 10];

    display(ssd);
}

