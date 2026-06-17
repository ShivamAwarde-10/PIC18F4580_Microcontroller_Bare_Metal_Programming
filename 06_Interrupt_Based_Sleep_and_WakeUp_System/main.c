/*
Implement system sleep and wake up while Interrupt on change (Shown in the video).

Output Requirements:
1] As soon as the board is powered up or reset, a message (say 1234) should be displayed in SSD.
2] A LED is toggled  every second.
3] System should go into sleep mode if no activity is detected.
4] System should wake up from sleep on detecting interrupt key.

Inputs:
1] INT0 as Interrupt to wake.
*/

#include <xc.h>
#include "main.h"
#include "sleep.h"
#include "digital_keypad.h"
#include "ssd_display.h"

void main(void) 
{
    unsigned char ssd[4] = {ONE, TWO, THREE, FOUR};
    
    unsigned int led_count = 0;
    
    unsigned char key;
    
    init_config();
    
    init_digital_keypad();
    
    init_ssd_control();
    
    while(1)
    {
       display(ssd);
       
       key = read_digital_keypad(EDGE);
       
       if(key != ALL_RELEASED)
       {
           timer = 0;
       }
       
       if(++led_count >= 100)
       {
           led_count = 0;
           
           toggle_led();
       }
       
       timer++;
       
       if(timer > 1000)
       {
           SLEEP();
           
           NOP();
       }
    } 
    return;
}
