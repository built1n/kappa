#include <stddef.h>
#include <stdio.h>
#include "isr.h"
#include "idt.h"
#include "panic.h"

void (*int_callbacks[256])(struct regs_t) = { NULL };

void set_interrupt_handler(uint8_t n, void (*callback)(struct regs_t))
{
    int_callbacks[n] = callback;
}

void isr_handler(struct regs_t regs)
{
    if(int_callbacks[regs.int_no])
    {
        int_callbacks[regs.int_no](regs);
    }
    else
    {
        printf("WARNING: unhandled ISR");
    }
}

void isr_init(void)
{
    printf("ISR handlers installed.\n");
    idt_set_gate(0, (uint32_t)_isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)_isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)_isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)_isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)_isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)_isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)_isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)_isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)_isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)_isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)_isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)_isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)_isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)_isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)_isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)_isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)_isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)_isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)_isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)_isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)_isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)_isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)_isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)_isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)_isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)_isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)_isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)_isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)_isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)_isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)_isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)_isr31, 0x08, 0x8E);
}
