/*
Write a Embedded C program to display the multiple patterns on the LEDs controlled by the switches. For every switch, dedicate a LEDs glow pattern. The pattern should change on key press.

Output Requirements:
Upon giving the power supply, all 8 LEDs should be OFF.

1] Now, press switch-1 on the Digital Keypad, LEDs should glow according to the pattern-1*
2] Press switch-2, LEDs should glow according to the pattern-2*
3] Press switch-3, LEDs should glow according to the pattern-3*
4] Press switch-4, LEDs should glow according to the pattern-4*

*pattern-1: The LEDs should glow from Left to Right and Right to left as explained in the assignment-1.
*pattern-2: The LEDs should glow from left to Right and switch off from left to right, no direction control/ direction change.
*pattern-3: The LEDs should blink alternately.
*pattern-4: The LEDs has to blink nibble wise, i.e first 4 LEDs will be ON, next 4 LEDs will be OFF, after first 4 LEDs will be OFF, next 4 LEDs will be ON.

Inputs:
Digital Keypad
*/

#include <xc.h>                  
#include "main.h"               
#include "digital_keypad.h"     

static void init_config(void) {
    ADCON1 = 0x0F;                            // Configure all pins as digital (disable analog mode)

    TRISB = 0x00;                             // Set PORTB as output for LEDs
    PORTB = 0x00;                             // Turn OFF all LEDs initially

    TRISC = TRISC | 0x0F;                     // Set RC0-RC3 as input for keypad
}

static void delay(void) {
    for (unsigned long int i = 100000; i--;);        // Simple software delay loop
}

void main(void) {
    unsigned char key;                               // Variable to store keypad input

    init_config();                                   // Initialize ports and peripherals
    init_digital_keypad();

    while (1) {
        key = read_digital_keypad(LEVEL);            // Read keypad continuously

        /* Pattern 1: Bidirectional train (left-right + right-left) */
        if (key == SW1) {
            unsigned char led;
            unsigned char i;

            led = 0x01;

            /* Left to Right */
            for (i = 0; i < 8; i++) {
                PORTB = led;
                delay();
                led = led << 1;
            }

            led = 0x40;

            /* Right to Left */
            for (i = 0; i < 7; i++) {
                PORTB = led;
                delay();
                led = led >> 1;
            }
        } else if (key == SW2) {
            unsigned char i;

            PORTB = 0x00;

            /* Turn ON Left -> Right */
            for (i = 0; i < 8; i++) {
                PORTB = (PORTB << 1) | 0x01;
                delay();
            }

            /* Turn OFF Left -> Right */
            for (i = 0; i < 8; i++) {
                PORTB = PORTB << 1;
                delay();
            }
        }                                          /* Pattern 3: Alternate blinking pattern */
        else if (key == SW3) {
            PORTB = 0xAA;                          // 10101010 pattern ON
            delay();                                

            PORTB = 0x55;                          // 01010101 pattern ON
            delay(); 
        }                                          /* Pattern 4: Nibble-wise blinking */
        else if (key == SW4) {
            PORTB = 0xF0;                          // Upper nibble ON (11110000)
            delay(); 

            PORTB = 0x0F;                          // Lower nibble ON (00001111)
            delay(); 
        }
    }
}
