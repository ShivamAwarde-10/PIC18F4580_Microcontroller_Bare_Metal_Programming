#include <xc.h>
#include "ssd_display.h"

void init_ssd_control(void)
{
    TRISD = 0x00;          // SSD data lines output

    TRISA = TRISA & 0xF0; // RA0-RA3 output

    PORTA = PORTA & 0xF0; // Disable all SSDs
}

void display(unsigned char data[])
{
    unsigned int wait;

    /* SSD0 */
    PORTD = data[0];
    PORTA = (PORTA & 0xF0) | 0x01;
    for(wait = 1000; wait--; );

    /* SSD1 */
    PORTD = data[1];
    PORTA = (PORTA & 0xF0) | 0x02;
    for(wait = 1000; wait--; );

    /* SSD2 */
    PORTD = data[2];
    PORTA = (PORTA & 0xF0) | 0x04;
    for(wait = 1000; wait--; );

    /* SSD3 */
    PORTD = data[3];
    PORTA = (PORTA & 0xF0) | 0x08;
    for(wait = 1000; wait--; );
}
