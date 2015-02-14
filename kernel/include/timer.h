#include <stdint.h>

#define HZ 100
#define PIT_FREQ 1193182

extern volatile const uint64_t *current_tick;

struct regs_t;

void timer_init(uint32_t freq);

/* NOTE: enables interrupts by default */
void timer_delay(uint64_t ticks);
