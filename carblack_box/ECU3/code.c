1] main.c

/*
 * File:   main.c
 * Author: shivam
 *
 * Created on 8 July, 2026, 9:56 AM
 */


#include <xc.h>
#include <stdint.h>
#include "can.h"
#include "clcd.h"
#include "msg_id.h"
#include "message_handler.h"
//#include "isr.h"
#include "timer0.h"

static void init_leds(void) 
{
    TRISB = 0x08;    // Set RB2 as output, RB3 as input, remaining as output
    PORTB = 0x00;    // Turn off all LEDs
}

static void init_config(void) {
    // Initialize CLCD and CANBUS
    init_clcd();
    init_can();
    init_leds();

    // Enable Interrupts
    PEIE = 1;
    GIE = 1;
    init_timer0();
}

void main(void) {
    // Initialize peripherals
    init_config();

    /* ECU1 main loop */
    while (1) {
        // Read CAN Bus data and process received messages
        process_canbus_data();
    }

    return;
}


2] message_handler.c

/*
 * File:   message_handler.c
 * Author: shivam
 *
 * Created on 8 July, 2026, 9:57 AM
 */


#include <xc.h>
#include <string.h>
#include "message_handler.h"
#include "msg_id.h"
#include "can.h"
#include "clcd.h"


volatile unsigned char led_state = LED_OFF;
volatile unsigned char status = e_ind_off;

// Handle Speed Message
void handle_speed_data(uint8_t *data, uint8_t len)
{
    char speed[5];

    memcpy(speed, data, len);
    speed[len] = '\0';

    clcd_print("SPD", LINE1(5));
    clcd_print(speed, LINE2(5));
}

// Handle Gear Message
void handle_gear_data(uint8_t *data, uint8_t len) 
{
    char *gear[] = {"N", "1", "2", "3", "4", "5", "R","C"};

    clcd_print("R", LINE1(9));
    clcd_print(gear[data[0]], LINE2(9));
}

// Handle RPM Message
void handle_rpm_data(uint8_t *data, uint8_t len) 
{
    char rpm[5];

    memcpy(rpm, data, len);
    rpm[len] = '\0';

    clcd_print("RPM", LINE1(0));
    clcd_print(rpm, LINE2(0));
}

// Handle Engine Temperature Message
void handle_engine_temp_data(uint8_t *data, uint8_t len) 
{
    //Implement the temperature function
}

// Handle Indicator Message
void handle_indicator_data(uint8_t *data, uint8_t len) 
{
    status = data[0];

    switch(status)
    {
        case e_ind_left:

            clcd_print("<-", LINE2(13));

            if(led_state == LED_ON)
            {
                LEFT_IND_ON();
            }
            else
            {
                LEFT_IND_OFF();
            }

            RIGHT_IND_OFF();

            break;

        case e_ind_right:

            clcd_print("->", LINE2(13));

            if(led_state == LED_ON)
            {
                RIGHT_IND_ON();
            }
            else
            {
                RIGHT_IND_OFF();
            }

            LEFT_IND_OFF();

            break;

        default:

            clcd_print("  ", LINE2(13));

            LEFT_IND_OFF();
            RIGHT_IND_OFF();

            break;
    }
}

void process_canbus_data() 
{   
    //process the CAN bus data
    uint16_t msg_id; 
    uint8_t data[8]; 
    uint8_t len;
    
    can_receive(&msg_id, data, &len);
    
    if(len != 0)
    {
       if(msg_id == SPEED_MSG_ID)
       {
           handle_speed_data(data, len);
       }
       
       if(msg_id == GEAR_MSG_ID)
       {
           handle_gear_data(data, len);
       }
       
       if(msg_id == RPM_MSG_ID)
       {
           handle_rpm_data(data, len);
       }
       
       if(msg_id == ENG_TEMP_MSG_ID)
       {
           handle_engine_temp_data(data, len);
       }
       
       if(msg_id == INDICATOR_MSG_ID)
       {
           handle_indicator_data(data, len);
       }
    
       len = 0;
    }
}

3] message_handler.h

#ifndef MESSAGE_HANDLER_H
#define	MESSAGE_HANDLER_H
#include <stdint.h>

#define LED_OFF 0
#define LED_ON 1

#define RIGHT_IND_ON() (PORTB = PORTB | 0xC0)
#define RIGHT_IND_OFF() (PORTB = PORTB & ~0xC0)
#define LEFT_IND_ON() (PORTB = PORTB | 0x03)
#define LEFT_IND_OFF() (PORTB = PORTB & ~0x03)

extern volatile unsigned char led_state, status;

typedef enum {
    e_ind_off,
    e_ind_left,
    e_ind_right
} IndicatorStatus;

void process_canbus_data();
void handle_speed_data(uint8_t *data, uint8_t len);
void handle_gear_data(uint8_t *data, uint8_t len);
void handle_rpm_data(uint8_t *data, uint8_t len);
void handle_engine_temp_data(uint8_t *data, uint8_t len);
void handle_indicator_data(uint8_t *data, uint8_t len);


#endif	/* MESSAGE_HANDLER_H */

