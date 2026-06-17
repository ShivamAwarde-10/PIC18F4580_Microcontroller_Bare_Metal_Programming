#include <xc.h>
#include "digital_keypad.h"
 
void init_digital_keypad(void)
{
    TRISC = TRISC | 0x0F;         // Sets RC0?RC3 bits to 1 (input mode) while leaving RC4?RC7 unchanged
}

unsigned char read_digital_keypad(unsigned char trigger)
{
    static unsigned char once = 1;
    
    if(trigger == EDGE)
    {
        if(((PORTC & 0x0F) != ALL_RELEASED) && once)
        {
            once = 0;
            
            return PORTC & 0x0F;
        }
        else if((PORTC & 0x0F) == ALL_RELEASED)
        {
            once = 1;
        }
    }
    return ALL_RELEASED;
}
