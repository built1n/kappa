#include <stddef.h>
#include <stdlib.h>
#include "multiboot.h"
#include "vga.h"

static uint8_t *framebuffer = NULL;
static uint16_t fb_width;
static uint16_t fb_height;

void vga_drawpixel(int x, int y, uint32_t pixel)
{
    ((uint32_t*)framebuffer)[y * fb_width + x] = pixel;
}

void vga_init(struct vbe_info_t *vbe_mode_info)
{
    framebuffer = vbe_mode_info->physbase;
    fb_width = vbe_mode_info->Xres;
    fb_height = vbe_mode_info->Yres;
}
