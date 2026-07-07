#include <xc.h>
#include "adc.h"
#include "can.h"
#include "ecu1_sensor.h"
#include "msg_id.h"
#include "uart.h"

void main(void)
{
    init_digital_keypad();
    init_adc();
    init_uart();

    // Array to store Gear names
    unsigned char *arr[8] = {"GN", "G1", "G2", "G3", "G4", "G5", "GR", "C"};

    while (1)
    {
        uint16_t speed = get_speed();          // Read current Speed

        unsigned char gear = get_gear_pos();   // Read current Gear Position
        
        puts("\n\r");                          // Display Speed
        puts("speed-> ");

        putch(speed / 10 + '0');
        putch(speed % 10 + '0');

        puts("\n\r");                          // Display Gear
        puts("gear -> ");

        puts(arr[gear]);
    }

    return;
}


#ifndef DIGITAL_KEYPAD_H
#define DIGITAL_KEYPAD_H

#define LEVEL					     	0
#define STATE_CHANGE			       	1

#define KEY_PORT					PORTC

#define SWITCH1					0x0E
#define SWITCH2					0x0D
#define SWITCH3					0x0B
#define SWITCH4					0x07
#define ALL_RELEASED					0x0F

#define INPUT_PINS					0x0F

void init_digital_keypad(void);
unsigned char read_digital_keypad(unsigned char detection_type);

#endif


#include "ecu1_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "uart.h"

// Read ADC value and convert it into Speed (0-99)
uint16_t get_speed()
{
    uint16_t speed = read_adc(CHANNEL4);

    speed = ((speed * 99) / 1023);            // Convert ADC value (0-1023) into Speed (0-99)
 
    return speed;
}

// Detect Gear Position using Digital Keypad
unsigned char get_gear_pos()
{
    static char gear = 0;                      // Static variable retains Gear value after function exits

    unsigned char key = read_digital_keypad(STATE_CHANGE);   // Read Keypad in State Change mode

    // Gear Up
    if (key == SWITCH1)
    {
        if (gear == 7)                        // If Collision Gear, return to Neutral
        {
            gear = 0;
        }
        else if (gear < 6)                    // Increase Gear up to Reverse
        {
            gear++;
        }
    }
    else if (key == SWITCH2)                  // Gear Down
    {
        if (gear == 7)                        // If Collision Gear, return to Neutral
        {
            gear = 0;
        }
        else if (gear > 0)                    // Decrease Gear until Neutral
        {
            gear--;
        }
    }
    else if (key == SWITCH3)                  // Collision
    {
        gear = 7;
    }

    return gear;
}
