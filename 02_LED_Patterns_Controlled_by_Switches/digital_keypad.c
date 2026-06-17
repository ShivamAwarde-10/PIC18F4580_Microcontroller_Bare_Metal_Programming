#include <xc.h>                 
#include "digital_keypad.h"     

void init_digital_keypad(void)
{
    TRISC = TRISC | 0x0F;                        // Set RC0-RC3 as input pins for keypad
}

unsigned char read_digital_keypad(unsigned char trigger)
{
    static int once = 1;                               // Static flag to detect single key press (EDGE control)

    if(trigger == LEVEL)                               // If LEVEL mode is selected
    {
        return PORTC & INPUT_PINS;                  // Continuously return current keypad status
    }
    else if(trigger == EDGE)                           // If EDGE mode is selected
    {
        if(((PORTC & INPUT_PINS) != ALL_RELEASED) && (once == 1))
        {
            once = 0;                                  // Mark key press detected once
            return PORTC & INPUT_PINS;              // Return pressed key value
        }
        else if((PORTC & INPUT_PINS) == ALL_RELEASED)
        {
            once = 1;                                  // Reset flag when all keys are released
        }
    }
    return ALL_RELEASED;                               // Default return when no key is pressed
}

