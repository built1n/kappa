#include <stdint.h>
#include <stdio.h>
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "multiboot.h"
#include "pcspkr.h"
#include "ps2.h"
#include "timer.h"
#include "tty.h"
#include "vga.h"

void main(struct multiboot_info_t *hdr, uint32_t magic)
{
    /* init the terminal first so we can get some output */
    tty_init();

    /* then the descriptor tables so we can do more useful stuff */
    gdt_init();
    idt_init();

    /* install all the interrupt stubs */
    isr_init();
    irq_init();

    /* initialize other drivers */
    timer_init();
    ps2_init();

    asm("sti");

    printf("Boot finished.\n");
    while(1)
    {
        ps2_set_leds(0x01);
        for(int i=0;i<5000000;++i);
        ps2_set_leds(0x02);
        for(int i=0;i<5000000;++i);
        ps2_set_leds(0x04);
        for(int i=0;i<5000000;++i);
        ps2_set_leds(0x02);
        for(int i=0;i<5000000;++i);
    }

}
