#include <stdint.h>

#define PS2_SCROLL_LOCK (1 << 0)
#define PS2_NUM_LOCK    (1 << 1)
#define PS2_CAPS_LOCK   (1 << 2)

void ps2_set_leds(uint8_t status);

void ps2_init(void);
