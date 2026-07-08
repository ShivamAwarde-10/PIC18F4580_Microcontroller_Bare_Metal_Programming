1] ecu2_main.c

/*
 * File:   ecu1_main.c
 * Author: shivam
 *
 * Created on 6 July, 2026, 8:25 AM
 */


#include <xc.h>
#include "digital_keypad.h"
#include "adc.h"
#include "uart.h"
#include "can.h"
#include "msg_id.h"
#include "ecu2_sensor.h"

void main(void)
{
    init_digital_keypad();
    init_adc();
    init_uart();
    init_can();
    
    unsigned char str[5];
    unsigned char ind_data[1];

    while (1)
    {
        uint16_t rpm = get_rpm();        // Read current RPM

        IndicatorStatus status = process_indicator();             // Process Indicator based on Keypad input

        // Convert RPM into ASCII
        str[0] = ((rpm / 1000) % 10 + '0');
        str[1] = ((rpm / 100) % 10 + '0');
        str[2] = ((rpm / 10) % 10 + '0');
        str[3] = ((rpm % 10) + '0');
        str[4] = '\0';
        
        // send RPM over CAN
        can_transmit(RPM_MSG_ID, str, 4);
        
        // Send Indicator Status over CAN
        ind_data[0] = status;
        can_transmit(INDICATOR_MSG_ID, ind_data, 1);
    }

    return;
}


2] ecu2_sensor.c

/*
 * File:   ecu1_sensor.c
 * Author: shivam
 *
 * Created on 6 July, 2026, 8:24 AM
 */


#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "uart.h"

volatile IndicatorStatus prev_ind_status = e_ind_off;
volatile IndicatorStatus cur_ind_status = e_ind_off;
volatile unsigned char led_state = LED_OFF;

// Read ADC value and convert it into RPM (0-6000)
uint16_t get_rpm()
{
    uint32_t rpm = read_adc(CHANNEL4);  // Read ADC value from Channel 4

    rpm = (rpm * 6000) / 1023;          // Convert ADC value (0-1023) into RPM (0-6000)

    return rpm;
}

//uint16_t get_engine_temp()
//{
//    //Implement the engine temperature function
//}

IndicatorStatus process_indicator()
{
    unsigned char key = read_digital_keypad(STATE_CHANGE); // Read Keypad in State Change mode

    prev_ind_status = cur_ind_status;
    
    if (key == SWITCH1)      // Left Indicator
    {
        cur_ind_status = e_ind_left;
    }
    else if (key == SWITCH2) // Right Indicator
    {
        cur_ind_status = e_ind_right;
    }
    else if (key == SWITCH3) // Hazard Indicator
    {
        cur_ind_status = prev_ind_status;
    }
    else if (key == SWITCH4) // Turn OFF Indicator
    {
        cur_ind_status = e_ind_off;
    }
    
    return cur_ind_status;
}


3] ecu2_sensor.h

#ifndef ECU2_SENSOR_H
#define	ECU2_SENSOR_H

#include <stdint.h>
#include "digital_keypad.h"
#include <xc.h>

#define RPM_ADC_CHANNEL 0x04
#define ENG_TEMP_ADC_CHANNEL 0x06

#define LED_OFF 0
#define LED_ON 1

#define RIGHT_IND_ON() (PORTB = PORTB | 0xC0)
#define RIGHT_IND_OFF() (PORTB = PORTB & ~0xC0)
#define LEFT_IND_ON() (PORTB = PORTB | 0x03)
#define LEFT_IND_OFF() (PORTB = PORTB & ~0x03)

typedef enum {
    e_ind_off,
    e_ind_left,
    e_ind_right,
    e_ind_hazard
} IndicatorStatus;

extern volatile IndicatorStatus prev_ind_status, cur_ind_status;
extern volatile unsigned char led_state;

uint16_t get_rpm();
uint16_t get_engine_temp();
IndicatorStatus process_indicator();

#endif	/* ECU1_SENSOR_H */


4] msg_id.h

#ifndef MSG_ID_H
#define	MSG_ID_H

#define SPEED_MSG_ID 0x10
#define GEAR_MSG_ID 0x20
#define RPM_MSG_ID 0x30
#define ENG_TEMP_MSG_ID 0x40
#define INDICATOR_MSG_ID 0x50

#endif	/* MSG_ID_H */

