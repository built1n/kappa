/* this is both a PS/2 keyboard AND a PS/2 MOUSE driver */
#ifndef _PS2KBD_H_
#define _PS2KBD_H_

#include <stdbool.h>
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

struct ps2_specialkeys_t {
    int shift      :1;
    int ctrl       :1;
    int bksp       :1;
    int alt        :1;
    int gui        :1;
    int numlock    :1;
    int capslock   :1;
    int scrllock   :1;
    int uparrow    :1;
    int downarrow  :1;
    int leftarrow  :1;
    int rightarrow :1;
    int esc        :1;
    int f1         :1;
    int f2         :1;
    int f3         :1;
    int f4         :1;
    int f5         :1;
    int f6         :1;
    int f7         :1;
    int f8         :1;
    int f9         :1;
    int f10        :1;
    int f11        :1;
    int f12        :1;
};

struct ps2_keyevent {
    const struct ps2_specialkeys_t *special_keys;
    char ascii;
};

/* returns which arrow keys are down */
uint8_t ps2kbd_button_get(void);

uint8_t ps2kbd_modifier_get(void);

void ps2kbd_set_leds(uint8_t status);

void ps2kbd_set_handler(void (*h)(const struct ps2_keyevent*));

void ps2kbd_init(void);

#endif
