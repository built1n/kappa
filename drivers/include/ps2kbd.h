/* this is both a PS/2 keyboard AND a PS/2 MOUSE driver */

#include <stdint.h>

#define PS2_SCROLL_LOCK (1 << 0)
#define PS2_NUM_LOCK    (1 << 1)
#define PS2_CAPS_LOCK   (1 << 2)

#define BUTTON_UP      (1 << 0)
#define BUTTON_LEFT    (1 << 1)
#define BUTTON_DOWN    (1 << 2)
#define BUTTON_RIGHT   (1 << 3)

#define MODIFIER_NONE  0
#define MODIFIER_SHIFT (1 << 0)
#define MODIFIER_CTRL  (1 << 1)
#define MODIFIER_ALT   (1 << 2)

/* returns which arrow keys are down */
uint8_t ps2kbd_button_get(void);

uint8_t ps2kbd_modifier_get(void);

void ps2kbd_set_leds(uint8_t status);

void ps2kbd_init(void);
