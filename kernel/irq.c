#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "idt.h"
#include "io.h"
#include "irq.h"
#include "isr.h"

/* in isr.c */
extern void *int_callbacks[256];

void irq_remap(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void irq_init(void)
{
    irq_remap();
    printf("IRQ handlers installed.\n");
    idt_set_gate(32, (uint32_t)_irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)_irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)_irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)_irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)_irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)_irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)_irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)_irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)_irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)_irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)_irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)_irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)_irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)_irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)_irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)_irq15, 0x08, 0x8E);
}

void irq_handler(struct regs_t regs)
{
    void (*handler)(struct regs_t r);

    handler = int_callbacks[regs.int_no];

    if(handler)
    {
        handler(regs);
    }
    else
    {
        printf("WARNING: Unhandled IRQ: 0x%x!\n", regs.int_no);
    }

    /* If the IDT entry that was invoked was greater than 40
     *  (meaning IRQ8 - 15), then we need to send an EOI to
     *  the slave controller */
    if (regs.int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    /* send an EOI to the master controller */
    outb(0x20, 0x20);
}
