/*
Implement a left scrolling number marquee on SSDs.

Output Requirements:
1] As soon as the board is powered up or reset, a static message (hardcoded) should start scrolling 
   from right to left on SSD.
2] The scroll should repeat indefinitely.
3] The scroll frequency should be 0.5Hz (Approximately, Non Timer Based).

Inputs:
Static number message in code (10 digit number + 2 spaces)

*/

#include <xc.h>
#include "ssd_display.h"

void main(void) 
{
    init_ssd_control();
    int i=0;
    int j=1;
    int k=2;
    int l=3;
    unsigned int delay = 0;
    unsigned char ssd[4];
    unsigned char arr[12] = {0xE7, 0x21, 0xCB, 0x6B, 0x2D, 0x6E, 0xEE, 0x23, 0xEF, 0x6F, 0x40, 0x40};
    
    while(1)
    {
        if(delay++ == 100)
        {
            ssd[0] = arr[i++%12];
            ssd[1] = arr[j++%12];
            ssd[2] = arr[k++%12];
            ssd[3] = arr[l++%12];
            delay=0;
        }
        display(ssd);
    }
    return;
}
