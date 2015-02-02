#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "tty.h"
#include <stdio.h>

void interrupt(struct regs_t regs)
{
}

void main(struct multiboot_header *hdr, uint32_t magic)
{
    tty_init();

    gdt_init();
    idt_init();

    isr_init();
    irq_init();

    for(int i=0;i<256;++i)
        set_interrupt_handler(i, interrupt);

    asm("sti");

    printf("Boot finished.!\n");
    while(1)
        ;
}
