#include <xc.h>
#include "ssd.h"

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
