#include <stdint.h>
#include <stdio.h>
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "log.h"
#include "multiboot.h"
#include "pcspkr.h"
#include "ps2.h"
#include "timer.h"
#include "tty.h"
#include "vga.h"

void main(struct multiboot_info_t *hdr, uint32_t magic)
{
    klog("main() called\n");

    /* initialize the TTY first, no real harm can be done */
    tty_init();

    klog("tty done, magic %x\n", magic);
    if(magic != 0x2BADB002)
    {
        panic("Multiboot magic invalid");
    }

    vga_init((struct vbe_info_t*)hdr->vbe_mode_info);

    klog("multiboot header address: %x\n", hdr);
    klog("multiboot vbe info struct address: %x\n", hdr->vbe_mode_info);
    klog("multiboot framebuffer: %x\n", ((struct vbe_info_t*)hdr->vbe_mode_info)->physbase);

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
    printf("Testing keyboard LED's...\n");
    for(int i=0;i<50;++i)
        vga_drawpixel(i, i, 0x80808080);
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
