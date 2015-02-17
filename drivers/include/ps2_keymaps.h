#include <stdint.h>

#define PS2_KEY_UNKNOWN  0xFF00
#define PS2_KEY_ESCAPE   0xFF01
#define PS2_KEY_NUMLOCK  0xFF02
#define PS2_KEY_CAPSLOCK 0xFF03
#define PS2_KEY_SCRLLOCK 0xFF04
#define PS2_KEY_CONTROL  0xFF05
#define PS2_KEY_BKSP     0xFF06
#define PS2_KEY_F1       0xFF07
#define PS2_KEY_F2       0xFF08
#define PS2_KEY_F3       0xFF09
#define PS2_KEY_F4       0xFF0A
#define PS2_KEY_F5       0xFF0B
#define PS2_KEY_F6       0xFF0C
#define PS2_KEY_F7       0xFF0D
#define PS2_KEY_F8       0xFF0E
#define PS2_KEY_F9       0xFF0F
#define PS2_KEY_F10      0xFF10
#define PS2_KEY_F11      0xFF11
#define PS2_KEY_F12      0xFF12
#define PS2_KEY_ALT      0xFF13
#define PS2_KEY_GUI      0xFF14
#define PS2_KEY_SHIFT    0xFF15

extern uint16_t ps2_keymap_set1[128];
