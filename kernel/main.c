#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "gdt.h"
#include "gfx.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "log.h"
#include "multiboot.h"
#include "panic.h"
#include "pcspkr.h"
#include "ps2.h"
#include "timer.h"
#include "tty.h"

void main(struct multiboot_info_t *hdr, uint32_t magic)
{
    /* this should go to port e9, which is the Bochs debug port */
    printf("Testing early I/O\n");

    printf("GFX init\n");
    int gfx_status = gfx_init((struct vbe_info_t*)hdr->vbe_mode_info);

    /* if graphical initialization fails, fall back to text mode */
    if(!gfx_status)
        tty_init();

    if(magic != 0x2BADB002)
    {
        panic("Multiboot magic invalid");
    }

    /* then the descriptor tables so we can do more useful stuff */
    gdt_init();
    idt_init();

    /* install all the interrupt stubs */
    isr_init();
    irq_init();

    /* initialize other drivers */
    timer_init(HZ);
    ps2_init();

    asm("sti");

    printf("Boot finished.\n");

    printf("Testing RNG...\n");
    srand(*current_tick);

    for(int i=0;i>=0;++i)
    {
        int rx = rand() % *gfx_width;
        int ry = rand() % *gfx_height;

        gfx_drawpixel(rx, ry, rand() % 0xFFFFFF);
    }
    gfx_clear(0xffffff);

    printf("Testing keyboard LED's...\n");

    while(1)
    {
        ps2_set_leds(PS2_NUM_LOCK);
        for(int i=0;i<50000000;++i);
        ps2_set_leds(PS2_CAPS_LOCK);
        for(int i=0;i<50000000;++i);
        ps2_set_leds(PS2_SCROLL_LOCK);
        for(int i=0;i<50000000;++i);
        ps2_set_leds(PS2_CAPS_LOCK);
        for(int i=0;i<50000000;++i);
    }
}
