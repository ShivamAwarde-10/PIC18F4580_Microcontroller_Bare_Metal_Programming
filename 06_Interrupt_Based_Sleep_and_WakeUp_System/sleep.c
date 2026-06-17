#include <xc.h>
#include "main.h"
#include "sleep.h"

volatile unsigned long timer = 0;

void init_external_interrupt(void)
{
    INTEDG0 = 1;
    
    INT0F = 0;
    
    INT0E = 1;
    
    GIE = 1;
}

void toggle_led(void)
{
    RB2 = !RB2;
}

void __interrupt() isr(void)
{
    if(INT0F)
    {
        timer = 0;
        
        INT0F = 0;
    }
}

void init_config(void)
{
    ADCON1 = 0;                // Configure pins as analog 

    TRISB2 = 0;                // Configure RB2 as output pin 

    RB2 = 0;                   // Initially turn OFF LED connected to RB2

    TRISB0 = 1;                // Configure RB0 as input pin (INT0 external interrupt pin)

    init_external_interrupt(); // Configure and enable INT0 interrupt
}
