/*
Write an Embedded C program to display a pattern on the LEDs.

Output Requirements:

1. Consider 8 LEDs from the Board, these LEDs has to glow one by one as if a train bogie coming 
   out of a tunnel every second (approx., none without timer implementation)from Left to Right as soon 
   as the board is powered on or reset (Let’s call it as exit)
2. Once all LEDs are ON, the LEDs should switch OFF one by one as if the train bogie is entering 
   inside a tunnel from Left to Right (Let’s call it as entry).
3. Once the exit and entry is complete , Change the direction of the train from Right to Left.
4. This process of changing direction has to be repeated between every exit and entry of the train.
Inputs:
No external inputs
Should be software controlled
 */


#include <xc.h>                 
#pragma config WDT = OFF                // Disable Watchdog Timer to prevent unexpected resets

void main(void) 
{
    TRISB = 0x00;                        // Set PORTB as output 
    PORTB = 0x00;                        // all LEDs are OFF initially on PORTB
    
    int count = 0;                       // Counter variable 
    unsigned long int wait = 0;          // Software delay counter 

    while(1)                    
    {
        if(wait++ == 100000)             // Create delay (approx 1 second depending on clock)
        {
            if(count >= 0 && count <= 7)            // Phase 1: Left to Right LED ON (train exits tunnel)
            {
                PORTB = PORTB << 1 | 1;             // Shift LEDs left and turn ON next LED (bitwise OR with 1)
            }
            else if(count >= 8 && count <= 15)      // Phase 2: Left to Right OFF (train entering tunnel)
            {
                PORTB = PORTB << 1;                 // Shift LEDs left, gradually turning OFF LEDs
            }
            else if(count >= 16 && count <= 23)     // Phase 3: Right to Left LED ON (reverse exit)
            {
                PORTB = PORTB >> 1 | 0x80;          // Shift right and force MSB (RB7) ON using 0x80
            }
            else if(count >= 24 && count <= 31)     // Phase 4: Right to Left OFF (reverse entry)
            {
                PORTB = PORTB >> 1;                 // Shift right, turning OFF LEDs one by one
            }
            else
                count = -1;                         // Reset cycle when count exceeds 31

            count++;                                // Move to next state in sequence
            wait = 0;                               // Reset delay counter for next cycle step
        }
    }
}
