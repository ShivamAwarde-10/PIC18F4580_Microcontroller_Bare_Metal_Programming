#ifndef DIGITAL_KEYPAD_H       // Prevent multiple inclusion of keypad header
#define DIGITAL_KEYPAD_H

#include <xc.h>                // Microcontroller register definitions

/* Trigger types */
#define LEVEL   0             // LEVEL trigger: continuous key reading
#define EDGE    1             // EDGE trigger: detects key press event only once

/* Switch definitions (active low keypad) */
#define SW1  0x0E             // Switch 1 pressed pattern (1110)
#define SW2  0x0D             // Switch 2 pressed pattern (1101)
#define SW3  0x0B             // Switch 3 pressed pattern (1011)
#define SW4  0x07             // Switch 4 pressed pattern (0111)

#define ALL_RELEASED 0x0F     // All switches released state (1111)

void init_digital_keypad(void);   // Function prototype for keypad init
unsigned char read_digital_keypad(unsigned char trigger); // Read keypad function

#endif                         
