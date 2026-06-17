#ifndef SSD_DISPLAY_H
#define SSD_DISPLAY_H

#define ONE     0x21
#define TWO     0xCB
#define THREE   0x6B
#define FOUR    0x2D

void init_ssd_control(void);

void display(unsigned char data[]);

#endif
