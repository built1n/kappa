#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "tty.h"
#include <stdio.h>

void irq1(struct regs_t *regs)
{
    printf("Keypress\n");
}

void main(struct multiboot_header *hdr, uint32_t magic)
{
    tty_init();

    gdt_init();
    printf("GDT initialized.\n");

    idt_init();
    printf("IDT initialized.\n");

    isr_init();
    irq_init();

    irq_set_handler(1, irq1);

    printf("Hello, world!\n");
    while(1)
        ;
}
