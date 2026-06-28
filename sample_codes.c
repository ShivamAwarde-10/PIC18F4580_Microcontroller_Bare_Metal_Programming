1] interrupt method
/*
 * File:   main.c
 * Author: shivam
 *
 * Created on 15 June, 2026, 10:52 AM
 */


#include <xc.h>

void init_external_Interrupt(void);

// Function to initialize LED pin and External Interrupt
void init_config()
{
    TRISB1 = 0x00;                 // Configure RB1 as OUTPUT (LED connected to RB1)
    
    RB1 = 0;                    // Initially turn OFF the LED
    
    TRISC = TRISC | 0x0F;       // Configure RC0-RC3 as INPUT pins
    
    TRISB0 = 1;                 // Configure RB0/INT0 pin as INPUT (External Interrupt pin)
    
    init_external_Interrupt();  // Configure and enable INT0 interrupt
}

// Function to configure External Interrupt INT0
void init_external_Interrupt()
{
    INTEDG0 = 1;     // Generate interrupt on Rising Edge (0 -> 1 transition)

    INT0F = 0;       // Clear INT0 interrupt flag before enabling interrupt

    INT0E = 1;       // Enable INT0 external interrupt

    GIE = 1;         // Enable Global Interrupts

    PEIE = 1;        // Enable Peripheral Interrupts
}

// CPU automatically jumps here whenever an enabled interrupt occurs
void __interrupt() isr(void)
{
    if(INT0F == 1)     // Check INT0 caused the interrupt
    {
        RB1 = !RB1;    // Toggle LED state
        
        INT0F = 0;     // Clear interrupt flag to allow next interrupt
    }
}

void main(void) 
{
    init_config();
    
    while(1)
    {
        // CPU stays here until an interrupt occurs
        for(int i=50; i--;)
            for(unsigned int delay = 50000; delay--;);
    }
    return;
}


2] polling method

/*
 * File:   main.c
 * Author: shivam
 *
 * Created on 15 June, 2026, 10:19 AM
 */


#include <xc.h>

// Function to initialize LED and Switch pins
void init_config()
{
    TRISB1 = 0x00;              // Configure RB1 pin as OUTPUT
    RB1 = 0;                 // Initially turn OFF the LED
    TRISC = TRISC | 0x0F;    // Configure RC0-RC3 as INPUT pins for switches
}

void main(void) 
{
    int once = 1;                      // Flag variable used for edge detection
    
    init_config();
    
    while(1)
    {
        // If RC0 switch is pressed and it is a new key press
        if(!RC0 && once)
        {
            RB1 = !RB1;               // Toggle LED state
            once = 0;
        }
        else if(RC0 == 1)             // Check switch is released
        {
            once = 1;                 // Re-enable key detection for next press
        }
        for(unsigned int i=50; i--;)  // Software delay
        {
            for(unsigned int i=50000; i--;);
        }
    }
    return;
}



3] timer0

/*
 * File:   main.c
 * Author: shivam
 *
 * Created on 17 June, 2026, 10:57 AM
 */


#include <xc.h>
#include "timer0.h"

void init_config(void)
{
    TRISB = 0x00;
    PORTB = 0x00;

    init_timer0();

    GIE = 1;
    PEIE = 1;
}

void main(void)
{
    init_config();

    while(1)
    {
        if(flag == 1)
        {
            PORTB = ~PORTB;
            flag = 0;
        }
    }
}

/*
 * File:   timer0.c
 * Author: shivam
 *
 * Created on 17 June, 2026, 10:59 AM
 */

#ifndef TIMER0_H
#define TIMER0_H

#include <xc.h>

/* Global flag used between ISR and main */
extern volatile int flag;

/* Function Prototypes */
void init_timer0(void);

#endif

#include <xc.h>
#include "timer0.h"

volatile int flag = 0;

void init_timer0(void)
{
    T08BIT = 1;      // 8-bit mode
    T0CS = 0;        // Internal clock
    PSA = 1;         // Prescaler not assigned

    TMR0 = 6;
    TMR0IF = 0;
    TMR0IE = 1;

    TMR0ON = 1;
}

void __interrupt() isr(void)
{
    static unsigned long int count = 0;

    if(TMR0IF)
    {
        if(++count >= 20000)
        {
            flag = 1;
            count = 0;
        }

        TMR0IF = 0;
        TMR0 += 8;
    }
}


4] timer1

#ifndef TIMER1_H
#define TIMER1_H

#include <xc.h>

/* Global flag used between ISR and main */
extern volatile int flag;

/* Function Prototypes */
void init_timer1(void);

#endif


/*
 * File:   main.c
 * Author: shivam
 *
 * Created on 22 June, 2026, 8:26 AM
 */


#include <xc.h>
#include "timer1.h"

void init_config(void)
{
    TRISB = 0x00;
    PORTB = 0x00;

    init_timer1();
}

void main(void)
{
    init_config();

    while(1)
    {
        if(flag == 1)
        {
            PORTB = ~PORTB;
            flag = 0;
        }
    }
}


/*
 * File:   timer1.c
 * Author: shivam
 *
 * Created on 22 June, 2026, 8:26 AM
 */


#include <xc.h>
#include "timer1.h"

volatile int flag = 0;

void init_timer1(void)
{
    //RD16 = 1;           
    TMR1 = 3036;        
    TMR1IE = 1;         
    TMR1IF = 0;
    TMR1ON = 1;
    
    GIE = 1;
    PEIE = 1;
}

void __interrupt() isr(void)
{
    static unsigned long int count = 0;

    if(TMR1IF)
    {
        TMR1 = TMR1 + 3036;
        if(++count == 80)
        {
            flag = 1;
            count = 0;
        }

        TMR1IF = 0;
    }
}



5] timer2

#ifndef TIMER2_H
#define TIMER2_H

#include <xc.h>

/* Global flag used between ISR and main */
extern volatile int flag;

/* Function Prototypes */
void init_timer2(void);

#endif /* TIMER2_H */


/*
 * File:   main.c
 * Author: shivam
 *
 * Created on 19 June, 2026, 10:05 AM
 */


#include <xc.h>
#include "timer2.h"

void init_config(void)
{
    TRISB = 0x00;
    PORTB = 0x00;

    init_timer2();

    GIE = 1;
    PEIE = 1;
}

void main(void)
{
    init_config();

    while(1)
    {
        if(flag == 1)
        {
            PORTB = ~PORTB;
            flag = 0;
        }
    }
}


/*
 * File:   timer2.c
 * Author: shivam
 *
 * Created on 19 June, 2026, 10:06 AM
 */


#include <xc.h>

volatile int flag = 0;

void init_timer2(void)
{    
    PR2 = 250;
    TMR2IF = 0;
    TMR2IE = 1;

    TMR2ON = 1;
}

void __interrupt() isr(void)
{
    static unsigned long int count = 0;

    if(TMR2IF)
    {
        if(++count >= 20000)
        {
            flag = 1;
            count = 0;
        }

        TMR2IF = 0;
    }
}


6] eeprome

