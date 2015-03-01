#include <stdint.h>

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry idt[0x100];
struct idt_ptr idt_pt;

void idt_init(void);

extern void idt_flush(uint32_t);

void idt_set_gate(uint8_t idx, uint32_t base, uint16_t sel, uint8_t flags);
