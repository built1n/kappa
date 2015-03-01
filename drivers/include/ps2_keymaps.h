#include <stdint.h>

#define EXTENDED_SCANCODE 0xE0

#define ERROR_KEY     0
#define PRINTING_KEY  1
#define SPECIAL_KEY   2

#define SPECIAL_NONE     0
#define SPECIAL_SHIFT    1
#define SPECIAL_CTRL     2
#define SPECIAL_BKSP     3
#define SPECIAL_ALT      4
#define SPECIAL_GUI      5
#define SPECIAL_NUMLOCK  6
#define SPECIAL_CAPLOCK  7
#define SPECIAL_SCRLLOCK 8
#define SPECIAL_UPARROW  9
#define SPECIAL_DNARROW  10
#define SPECIAL_LFTARROW 11
#define SPECIAL_RTARROW  12
#define SPECIAL_ESC      13
#define SPECIAL_F1       21
#define SPECIAL_F2       22
#define SPECIAL_F3       23
#define SPECIAL_F4       24
#define SPECIAL_F5       25
#define SPECIAL_F6       26
#define SPECIAL_F7       27
#define SPECIAL_F8       28
#define SPECIAL_F9       29
#define SPECIAL_F10      30
#define SPECIAL_F11      31
#define SPECIAL_F12      32

extern uint8_t ps2_set1_scancodes[128];
extern char ps2_set1_ascii[128];
extern char ps2_set1_shift[128];
extern uint8_t ps2_set1_special[128];
