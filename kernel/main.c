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
#include "fpu.h"
#include "timer.h"
#include "tty.h"

void gpf(struct regs_t regs)
{
    printf("General protection fault!\n");
    printf("EIP before fault: 0x%x\n", regs.eip);
    printf("Error code: 0x%x\n", regs.err_code);
    panic("GPF");
}

void main(struct multiboot_info_t *hdr, uint32_t magic)
{
    fpu_enable();
    asm("movq %xmm0, %xmm0");
    /* this should go to port e9, which is the Bochs debug port */
    printf("Testing early I/O\n");

    printf("GFX init\n");
    bool gfx_status = gfx_init((struct vbe_info_t*)hdr->vbe_mode_info);

    /* if graphical initialization fails, fall back to text mode */
    if(!gfx_status)
    {
        tty_init();
        printf("Graphics init failed, fell back to text mode.\n");
    }

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

    set_interrupt_handler(0xd, gpf);

    asm("sti");

    printf("Boot finished.\n");

    printf("Testing RNG...\n");
    srand(*current_tick);

    if(gfx_status)
    {
        int startpix = *current_tick;
        for(int i=0;i<1000000;++i)
        {
            int rx = rand() % *gfx_width;
            int ry = rand() % *gfx_height;

            gfx_set_foreground(rand() % 0x1000000);
            gfx_drawpixel(rx, ry);
        }
        int endpix = *current_tick;

        int startfill = *current_tick;
        for(int i=0;i<1000;++i)
        {
            gfx_set_background(rand() % 0x1000000);
            gfx_clear();
        }
        int endfill = *current_tick;

        gfx_set_background(0);
        gfx_set_foreground(0xFFFFFF);
        gfx_clear();
        int starttext = *current_tick;
        for(int i=0;i<1000000;++i)
        {
            int rx = rand() % *gfx_width;
            int ry = rand() % *gfx_height;
            gfx_drawchar(rx, ry, rand()%127+1);
        }
        int endtext = *current_tick;
        gfx_clear();

        int starthline = *current_tick;
        for(int i=0;i<1000000;++i)
        {
            gfx_hline(rand() % *gfx_width, rand() % *gfx_width, rand() % *gfx_height);
        }
        int endhline = *current_tick;

        gfx_clear();

        int startvline = *current_tick;
        for(int i=0;i<1000000;++i)
        {
            gfx_vline(rand() % *gfx_height, rand() % *gfx_height, rand() % *gfx_width);
        }
        int endvline = *current_tick;

        int startrect = *current_tick;
        for(int i=0;i<1000;++i)
        {
            int x = rand() % *gfx_width;
            int y = rand() % *gfx_height;
            int w = rand() % (*gfx_width - x);
            int h = rand() % (*gfx_height - y);
            gfx_fillrect(x, y, w, h);
        }
        int endrect = *current_tick;

        gfx_reset();
        printf("--- Graphics benchmark results ---\n");
        printf("Ticks for 1,000,000 random pixels: %d\n", endpix-startpix);
        printf("Ticks for 1,000 random fills:      %d\n", endfill-startfill);
        printf("Ticks for 1,000,000 random chars:  %d\n", endtext-starttext);
        printf("Ticks for 1,000,000 random hlines: %d\n", endhline-starthline);
        printf("Ticks for 1,000,000 random vlines: %d\n", endvline-startvline);
        printf("Ticks for 1,000 random rects:      %d\n", endrect-startrect);
        printf("Ticks per second:                  %d\n", HZ);
        printf("Resolution: %dx%dx%d\n", *gfx_width, *gfx_height, *gfx_bpp * 8);
    }

    printf("Testing keyboard LED's...\n");

    while(1)
    {
        ps2_set_leds(PS2_NUM_LOCK);
        timer_delay(HZ/4);
        ps2_set_leds(PS2_CAPS_LOCK);
        timer_delay(HZ/4);
        ps2_set_leds(PS2_SCROLL_LOCK);
        timer_delay(HZ/4);
        ps2_set_leds(PS2_CAPS_LOCK);
        timer_delay(HZ/4);
    }
}
