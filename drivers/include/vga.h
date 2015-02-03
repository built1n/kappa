#include <stdint.h>

enum vga_color_t {
    VGA_BLACK = 0,
    VGA_BLUE = 1,
    VGA_GREEN = 2,
    VGA_CYAN = 3,
    VGA_RED = 4,
    VGA_MAGENTA = 5,
    VGA_BROWN = 6,
    VGA_LIGHT_GRAY = 7,
    VGA_DARK_GRAY = 8,
    VGA_LIGHT_BLUE = 9,
    VGA_LIGHT_GREEN = 10,
    VGA_LIGHT_CYAN = 11,
    VGA_LIGHT_RED = 12,
    VGA_LIGHT_MAGENTA = 13,
    VGA_LIGHT_BROWN = 14,
    VGA_WHITE = 15
};

#define VGA_WIDTH  80
#define VGA_HEIGHT 25

#define VGA_MAKE_COLOR(fg, bg) (fg | bg << 4)
#define VGA_MAKE_ENTRY(ch, col) (((uint16_t)ch)|((uint16_t)col<<8))
