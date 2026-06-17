/*
Implement a 4 digit key press counter with persistence using Internal EEPROM. 

Output Requirements:
1] As soon as the board is powered up or reset, counter should display 0000 on SSDs.
2] On every key press  counter value should increment by 1.
3] On a  long key press (2 seconds), Count should  reset to zero.
4] On pressing STORE switch, the current count should be  stored in internal EEPROM.
5] On subsequent power ups or reset the counter should start from the previous stored value in the EEPROM.

Inputs:
1] DKS 1 as Count input .
2] DKS 1 Long press to reset the count .
3] DKS 2 Store Input.
*/

#include <xc.h>
#include "main.h"
#include "digital_keypad.h"
#include "ssd_display.h"
#include "eeprome.h"

void init_config(void) 
{
    ADCON1 = 0x0F;

    init_digital_keypad();
    init_ssd_control();
}

void main(void) 
{
    unsigned char key;                // Stores pressed switch.
    unsigned long press = 0;          // Counts how long SW1 is pressed.
    unsigned int count;               // Stores current counter value.

    init_config();

    /* Read saved count from EEPROM */
    count = read_internal_eeprom(COUNT_ADDR0);
    count = count | ((unsigned int) read_internal_eeprom(COUNT_ADDR1) << 8);

    while (1) 
    {
        key = read_digital_keypad(LEVEL);

        /* SW1 Count Input */
        if (key == SW1) 
        {
            press++;

            if (press > 200) 
            {
                press = 200;
            }
        } 
        else 
        {
            if (press >= 200) 
            {
                count = 0;
            } 
            else if (press > 0) 
            {
                count++;

                if (count > 9999) 
                {
                    count = 0;
                }
            }

            press = 0;
        }

        /* SW2 Store Count */
        if (read_digital_keypad(EDGE) == SW2) 
        {
            unsigned char low_byte;
            unsigned char high_byte;

            low_byte = count & 0x00FF;             // Lower 8 bits
            high_byte = (count >> 8) & 0x00FF;     // Upper 8 bits

            write_internal_eeprom(COUNT_ADDR0, low_byte);
            write_internal_eeprom(COUNT_ADDR1, high_byte);
        }

        display_count(count);
    }
}
