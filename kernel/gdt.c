#include "gdt.h"

static void gdt_set_gate(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    /* Setup the descriptor base address */
    gdt[idx].base_low = (base & 0xFFFF);
    gdt[idx].base_middle = (base >> 16) & 0xFF;
    gdt[idx].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[idx].limit_low = (limit & 0xFFFF);
    gdt[idx].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[idx].granularity |= (gran & 0xF0);
    gdt[idx].access = access;
}

void gdt_init(void)
{
    gp.limit = (sizeof(struct gdt_entry) * sizeof(gdt)/sizeof(gdt[0])) - 1;
    gp.base = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);

    /* code segment */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* data segment */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_flush((uint32_t)&gp);
}
