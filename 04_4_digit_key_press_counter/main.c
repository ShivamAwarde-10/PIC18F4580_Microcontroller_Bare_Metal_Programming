/*
Implement a 4 digit key press counter.

Output Requirement:
1] Implement a 4 digit key press counter
2] On every key press counter value should increment by 1.
3] On a long key press (2 seconds), count should reset to zero.

Inputs:
1] DKS1 (Digital Keypad Switch 1) as Count Input 
2] DKS1 long press (2 secs) to reset the count 
*/

#include <xc.h>
#include "main.h"
#include "digital_keypad.h"
#include "ssd.h"

static void init_config(void)
{
    ADCON1 = 0x0F;          // All pins digital

    init_digital_keypad();
    init_ssd_control();
}

void main(void)
{
    unsigned long int Press = 0;
    unsigned int Count = 0;
    unsigned char key;

    init_config();

    while(1)
    {
        key = read_digital_keypad(LEVEL);

        if(key == SW1)
        {
            Press++;

            if(Press > 200)
            {
                Press = 200;      // Saturate at long press value
            }
        }
        else
        {
            if(Press >= 200)
            {
                Count = 0;        // Long press reset
            }
            else if(Press > 0)
            {
                Count++;          // Short press increment

                if(Count > 9999)
                {
                    Count = 0;
                }
            }

            Press = 0;
        }

        display_count(Count);
    }
}

