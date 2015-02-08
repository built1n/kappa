#include <stdbool.h>
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
#define VGA_RGBPACK(r, g, b)  ((r << 16)|(g << 8)|(b << 0))

#define GFX_WHITE 0xFFFFFF
#define GFX_BLACK 0x000000

struct vbe_info_t;

bool gfx_init(struct vbe_info_t *vbe_mode_info);

void gfx_drawpixel(int x, int y, uint32_t color);

void gfx_clear(uint32_t color);

extern const uint16_t *gfx_width, *gfx_height;
