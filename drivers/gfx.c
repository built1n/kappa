#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfx_font.h"
#include "log.h"
#include "multiboot.h"
#include "panic.h"
#include "gfx.h"

uint8_t *framebuffer = NULL;
uint16_t fb_width;
uint16_t fb_height;

/* this is BYTES per pixel */
uint8_t  fb_bpp;
const uint8_t *gfx_bpp = &fb_bpp;

const uint16_t *gfx_width = &fb_width;
const uint16_t *gfx_height = &fb_height;

static int cursor_x, cursor_y;
uint32_t _gfx_fgcol, _gfx_bgcol;

void gfx_set_background(uint32_t col)
{
    _gfx_bgcol = col;
}

uint32_t gfx_get_background(void)
{
    return _gfx_bgcol;
}

void gfx_set_foreground(uint32_t col)
{
    _gfx_fgcol = col;
}

uint32_t gfx_get_foreground(void)
{
    return _gfx_fgcol;
}

void gfx_drawpixel(int x, int y)
{
    ((uint32_t*)framebuffer)[y * fb_width + x] = _gfx_fgcol;
}

/* implemented in assembly now */
/*
void gfx_clear(uint32_t col)
{
    uint8_t *p = framebuffer;
    uint8_t *stop = framebuffer + fb_width * fb_height * fb_bpp;
    while(p < stop)
    {
        *(uint32_t*)p = col;
        p += fb_bpp;
    }
}
*/

void gfx_reset(void)
{
    _gfx_fgcol = VGA_RGBPACK(0xff, 0xff, 0xff);
    _gfx_bgcol = VGA_RGBPACK(0, 0, 0);
    gfx_clear();
    cursor_y = 0;
    cursor_x = 0;
}

void gfx_drawchar(int x, int y, char c)
{
    int stride = fb_bpp * fb_width;
    uint8_t *line_addr = framebuffer + (x * fb_bpp) + (y * stride);
    for(int i = 0; i < FONT_HEIGHT; ++i)
    {
        uint32_t line_buf[8] = {_gfx_bgcol};
        uint8_t mask = 0x80;
        for(int j = 0; j < 8; ++j, mask >>= 1)
        {
            if(gfx_font[(int)c][i] & mask)
                line_buf[j] = _gfx_fgcol;
        }
        memcpy(line_addr, line_buf, sizeof(line_buf));
        line_addr += stride;
    }
}

void gfx_putchar(char ch)
{
    if(ch != '\n')
    {
        gfx_drawchar(cursor_x, cursor_y, ch);
        cursor_x += FONT_WIDTH;
        if(cursor_x >= fb_width)
        {
            cursor_x = 0;
            cursor_y += FONT_HEIGHT;
            if(cursor_y >= fb_height)
            {
                gfx_clear();
                cursor_y = 0;
            }
        }
    }
    else
    {
        cursor_x = 0;
        cursor_y += FONT_HEIGHT;
        if(cursor_y >= fb_height)
        {
            gfx_clear();
            cursor_y = 0;
        }
    }
}

void gfx_puts(const char* str)
{
    while(*str)
    {
        gfx_putchar(*str++);
    }
}

bool gfx_init(struct vbe_info_t *vbe_mode_info)
{
    framebuffer = (uint8_t*)vbe_mode_info->physbase;
    fb_width = vbe_mode_info->Xres;
    fb_height = vbe_mode_info->Yres;
    fb_bpp = vbe_mode_info->bpp / 8;
    if(fb_bpp != 4)
    {
        printf("WARNING: BPP != 32, falling back to text mode...\n");
        return false;
    }
    gfx_reset();
    set_putchar(gfx_putchar);
    set_puts(gfx_puts);

    return true;
}
