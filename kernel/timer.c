#include <stdint.h>
#include <stdio.h>
#include "io.h"
#include "isr.h"
#include "timer.h"

volatile uint64_t current_tick_data = 0;

volatile const uint64_t *current_tick = &current_tick_data;

static void timer_callback(struct regs_t *regs)
{
    (void) regs;
    ++current_tick_data;
}

void timer_init(uint32_t freq)
{
    set_interrupt_handler(IRQ(0), timer_callback);

    current_tick_data = 0;

    uint32_t divisor = PIT_FREQ / freq;

    uint8_t hi = (divisor >> 8) & 0xFF;
    uint8_t lo = (divisor >> 0) & 0xFF;

    outb(0x43, 0x36);
    outb(0x40, lo);
    outb(0x40, hi);
}

void timer_delay(uint64_t ticks)
{
    uint64_t end = *current_tick + ticks;
    while(*current_tick <= end)
    {
        asm("sti");
        asm("hlt");
    }
}
