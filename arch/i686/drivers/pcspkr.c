#include <stdint.h>
#include "io.h"
#include "pcspkr.h"

void pcspkr_play(uint32_t freq)
{
    uint32_t div;
    uint8_t tmp;

    div = 1193180 / freq;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t)(div));
    outb(0x42, (uint8_t)(div >> 8));

    tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}
