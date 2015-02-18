#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "fpu.h"
#include "gdt.h"
#include "gfx.h"
#include "gfx_font.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "log.h"
#include "multiboot.h"
#include "panic.h"
#include "pcspkr.h"
#include "ps2kbd.h"
#include "fpu.h"
#include "timer.h"
#include "tty.h"
#include "version.h"

void gpf(struct regs_t *regs)
{
    gfx_reset();
    printf("General protection fault!\n");
    printf("EIP before fault: 0x%x\n", regs->eip);
    printf("Error code: 0x%x\n", regs->err_code);
    panic("GPF!");
}

void int80(struct regs_t *regs)
{
    switch(regs->eax)
    {
    case 0:
        panic((const char*)regs->ebx);
        break;
    case 1:
        puts((const char*)regs->ebx);
        break;
    }
}

void div0(struct regs_t *regs)
{
    (void) regs;
    panic("Divide by zero!\n");
}

void main(struct multiboot_info_t *hdr, uint32_t magic)
{
    fpu_enable();

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
    ps2kbd_init();

    set_interrupt_handler(0, div0);
    set_interrupt_handler(0xd, gpf);
    set_interrupt_handler(0x80, int80);

    asm("sti");

    printf("Boot finished.\n");

    printf("Kernel version %s: \"%s\"\n", KAPPA_KERNEL_VERSION, KAPPA_KERNEL_CODENAME);

    printf("Starting linked-in application XRacer...\n");
    //printf("Running graphics benchmark...\n");
    srand(42);

    gfx_reset();

    enum plugin_status;
    struct plugin_api;
    extern enum plugin_status xracer_main(const struct plugin_api*);
    extern void plugin_load(enum plugin_status (*func)(const struct plugin_api*));

    plugin_load(xracer_main);

    gfx_filltriangle(100, 100, 100, 100, 100, 100);

    for(int i=0;i<200;)
    {
        gfx_putsxy_bg(i, 0, " ");
        gfx_drawchar_bg(++i, 0, 131);
        timer_delay(1);
    }
    while(1);

    if(gfx_status)
    {
        const int width = *gfx_width;
        const int height = *gfx_height;

        gfx_reset();

        int startpix = *current_tick;
        for(int i=0;i<1000000;++i)
        {
            int rx = rand() % width;
            int ry = rand() % height;

            gfx_set_foreground(rand() % 0x1000000);
            gfx_drawpixel(rx, ry);
        }
        int endpix = *current_tick;

        gfx_reset();

        int startfill = *current_tick;
        for(int i=0;i<1000;++i)
        {
            gfx_set_background(rand() % 0x1000000);
            gfx_clear();
        }
        int endfill = *current_tick;

        gfx_reset();

        int starttext = *current_tick;
        for(int i=0;i<1000000;++i)
        {
            int rx = rand() % width;
            int ry = rand() % height;
            gfx_set_foreground(rand() % 0x1000000);
            gfx_drawchar(rx, ry, rand()%127+1);
        }
        int endtext = *current_tick;

        gfx_reset();

        int starthline = *current_tick;
        for(int i=0;i<1000000;++i)
        {
            gfx_set_foreground(rand() % 0x1000000);
            gfx_hline(rand() % width, rand() % width, rand() % height);
        }
        int endhline = *current_tick;

        gfx_reset();

        int startvline = *current_tick;
        for(int i=0;i<1000000;++i)
        {
            gfx_set_foreground(rand() % 0x1000000);
            gfx_vline(rand() % height, rand() % height, rand() % width);
        }
        int endvline = *current_tick;

        gfx_reset();

        int startrect = *current_tick;
        for(int i=0;i<10000;++i)
        {
            int x = rand() % width;
            int y = rand() % height;
            int w = rand() % (width - x);
            int h = rand() % (height - y);
            gfx_set_foreground(rand() % 0x1000000);
            gfx_fillrect(x, y, w, h);
        }
        int endrect = *current_tick;

        gfx_reset();

        int startline = *current_tick;
        for(int i=0;i<1000000;++i)
        {
            int x1= rand() % width;
            int x2= rand() % width;
            int y1= rand() % height;
            int y2= rand() % height;
            gfx_set_foreground(rand() % 0x1000000);
            gfx_drawline(x1, y1, x2, y2);
            char buf[32];
            snprintf(buf, 32, "Line %d", i);
            gfx_putsxy(0, 0, buf);
        }
        int endline = *current_tick;

        gfx_reset();

        printf("--- Graphics benchmark results ---\n");
        printf("Ticks for 1,000,000 random pixels: %d\n", endpix-startpix);
        printf("Ticks for 1,000 random fills:      %d\n", endfill-startfill);
        printf("Ticks for 1,000,000 random chars:  %d\n", endtext-starttext);
        printf("Ticks for 1,000,000 random hlines: %d\n", endhline-starthline);
        printf("Ticks for 1,000,000 random vlines: %d\n", endvline-startvline);
        printf("Ticks for 10,000 random rects:     %d\n", endrect-startrect);
        printf("Ticks for 1,000,000 random lines:  %d\n", endline-startline);
        printf("Ticks per second:                  %d\n", HZ);
        printf("Resolution: %dx%dx%d\n", *gfx_width, *gfx_height, *gfx_bpp * 8);
    }

    printf("Testing keyboard LED's");

    int n = 0;
    int s = 1;

    while(1)
    {
        ps2kbd_set_leds(PS2_NUM_LOCK);
        timer_delay(HZ/4);
        if(s < 0)
            putchar('\b');
        else
            putchar('.');
        n+=s;
        if(n<=0 || n>=3)
            s=-s;
        ps2kbd_set_leds(PS2_CAPS_LOCK);
        timer_delay(HZ/4);
        if(s < 0)
            putchar('\b');
        else
            putchar('.');
        n+=s;
        if(n<=0 || n>=3)
            s=-s;
        ps2kbd_set_leds(PS2_SCROLL_LOCK);
        timer_delay(HZ/4);
        if(s < 0)
            putchar('\b');
        else
            putchar('.');
        n+=s;
        if(n<=0 || n>=3)
            s=-s;
        ps2kbd_set_leds(PS2_CAPS_LOCK);
        timer_delay(HZ/4);
        if(s < 0)
            putchar('\b');
        else
            putchar('.');
        n+=s;
        if(n<=0 || n>=3)
            s=-s;
    }
}
