#ifndef SSD_H
#define SSD_H

#define ZERO    0xE7
#define ONE     0x21
#define TWO     0xCB
#define THREE   0x6B
#define FOUR    0x2D
#define FIVE    0x6E
#define SIX     0xEE
#define SEVEN   0x23
#define EIGHT   0xEF
#define NINE    0x6F

void init_ssd_control(void);
void display(unsigned char data[]);
void display_count(unsigned int count);

#endif
