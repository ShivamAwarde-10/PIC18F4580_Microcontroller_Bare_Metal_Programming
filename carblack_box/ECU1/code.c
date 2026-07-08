1] ecu1_main.c

/*
 * File:   ecu1_main.c
 * Author: shivam
 *
 * Created on 6 July, 2026, 8:15 AM
 */


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
    init_can();

    // Array to store Gear names
    unsigned char *arr[8] = {"N", "1", "2", "3", "4", "5", "R", "C"};
    
    unsigned char speed_data[2];
    unsigned char gear_data[1];

    while (1)
    {
        uint16_t speed = get_speed();          // Read current Speed

        unsigned char gear = get_gear_pos();   // Read current Gear Position
        
        // Convert Speed into ASCII 
        speed_data[0] = (speed / 10) + '0';
        speed_data[1] = (speed % 10) + '0';
        
        // Store Gear 
        gear_data[0] = gear;
        
        // Transmit over CAN 
        can_transmit(SPEED_MSG_ID, speed_data, 2);
        can_transmit(GEAR_MSG_ID, gear_data, 1);
        
        puts("\n\r");                          // UART Display Speed 
        puts("speed-> ");

        putch(speed_data[0]);
        putch(speed_data[1]);

        puts("\n\r");                          // Display Gear
        puts("gear-> ");

        puts(arr[gear]);
    }

    return;
}


2] ecu1_sensor.c

/*
 * File:   ecu1_sensor.c
 * Author: shivam
 *
 * Created on 6 July, 2026, 8:14 AM
 */


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


3] ecu1_sensor.h

#ifndef ECU1_SENSOR_H
#define	ECU1_SENSOR_H

#include <stdint.h>
#include "digital_keypad.h"
#include <xc.h>

#define MAX_GEAR 6
#define SPEED_ADC_CHANNEL 0x04
#define GEAR_UP             SWITCH1
#define GEAR_DOWN           SWITCH2
#define COLLISION           SWITCH3

uint16_t get_speed();
unsigned char get_gear_pos();
//unsigned char get_collision_status();

#endif	/* ECU1_SENSOR_H */

