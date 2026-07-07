#include <xc.h>
#include "digital_keypad.h"
#include "adc.h"
#include "uart.h"
#include "ecu2_sensor.h"

void main(void)
{
    init_digital_keypad();
    init_adc();
    init_uart();

    while (1)
    {
        uint16_t rpm = get_rpm();        // Read current RPM

        process_indicator();             // Process Indicator based on Keypad input

        puts("\n\r");                    // Display RPM
        puts("rpm-> ");

        putch((rpm / 1000) % 10 + '0');
        putch((rpm / 100) % 10 + '0');
        putch((rpm / 10) % 10 + '0');
        putch((rpm % 10) + '0');
    }

    return;
}


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
    e_ind_right
} IndicatorStatus;

extern volatile IndicatorStatus prev_ind_status, cur_ind_status;
extern volatile unsigned char led_state;

uint16_t get_rpm();
uint16_t get_engine_temp();
IndicatorStatus process_indicator();

#endif	/* ECU1_SENSOR_H */


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

    if (key == SWITCH1)      // Left Indicator
    {

    }
    else if (key == SWITCH2) // Right Indicator
    {

    }
    else if (key == SWITCH3) // Hazard Indicator
    {

    }
    else if (key == SWITCH4) // Turn OFF Indicator
    {

    }
}
