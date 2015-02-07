#include <stddef.h>
#include <stdlib.h>
#include "log.h"
#include "multiboot.h"
#include "panic.h"
#include "vga.h"

static uint8_t *framebuffer = NULL;
static uint16_t fb_width;
static uint16_t fb_height;
/* this is BYTES per pixel */
static uint8_t  fb_bpp;

const uint16_t *vga_width = &fb_width;
const uint16_t *vga_height = &fb_height;

void vga_drawpixel(int x, int y, uint32_t col)
{
    ((uint32_t*)framebuffer)[y * fb_width + x] = col;
}

void vga_clear(uint32_t col)
{
    uint8_t *p = framebuffer;
    uint8_t *stop = framebuffer + fb_width * fb_height * fb_bpp;
    while(p < stop)
    {
        *(uint32_t*)p = col;
        p += fb_bpp;
    }
}

void vga_init(struct vbe_info_t *vbe_mode_info)
{
    framebuffer = (uint8_t*)vbe_mode_info->physbase;
    fb_width = vbe_mode_info->Xres;
    fb_height = vbe_mode_info->Yres;
    fb_bpp = vbe_mode_info->bpp / 8;
    if(fb_bpp != 4)
        panic("BPP *MUST* be 32!!!\n");
    vga_clear(VGA_RGBPACK(0, 0, 0));
}
