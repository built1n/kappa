#include <stdint.h>
#include <string.h>
#include "idt.h"

void idt_set_gate(uint8_t idx, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[idx].base_lo = base & 0xFFFF;
    idt[idx].base_hi = base >> 16;
    idt[idx].sel = sel;
    idt[idx].zero = 0;
    idt[idx].flags = flags;
}

void idt_init(void)
{
    idt_pt.limit = sizeof(idt) - 1;
    idt_pt.base = (uint32_t)&idt;

    memset(&idt, 0, sizeof(idt));

    idt_flush((uint32_t)&idt_pt);
}
