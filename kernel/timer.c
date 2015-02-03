#include <stdint.h>
#include "isr.h"
#include "timer.h"

uint64_t current_tick_data = 0;

const uint64_t *current_tick = &current_tick_data;

static void timer_callback(struct regs_t regs)
{
    (void) regs;
    ++current_tick_data;
}

void timer_init(void)
{
    set_interrupt_handler(IRQ(0), timer_callback);
}
