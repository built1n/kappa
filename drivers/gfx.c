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

static uint8_t *framebuffer = NULL;
static uint16_t fb_width;
static uint16_t fb_height;

/* this is BYTES per pixel */
static uint8_t  fb_bpp;

const uint16_t *gfx_width = &fb_width;
const uint16_t *gfx_height = &fb_height;

void gfx_drawpixel(int x, int y, uint32_t col)
{
    ((uint32_t*)framebuffer)[y * fb_width + x] = col;
}

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

void gfx_drawchar(int x, int y, char c, uint32_t fg, uint32_t bg)
{
    int stride = fb_bpp * fb_width;
    uint8_t *line_addr = framebuffer + (x * fb_bpp) + (y * fb_width * fb_bpp);
    for(int i = y; i < y + FONT_HEIGHT; ++i)
    {
        uint32_t line_buf[8] = {bg};
        uint8_t mask = 0x80;
        for(int j = 0; j < 8; ++j, mask >>= 1)
        {
            if(gfx_font[(int)c][i] & mask)
                line_buf[j] = fg;
        }
        memcpy(line_addr, line_buf, sizeof(line_buf));
        line_addr += stride;
    }
}

static int cursor_x, cursor_y;
static uint32_t fgcol, bgcol;

void gfx_putchar(char ch)
{
    if(ch != '\n')
    {
        gfx_drawchar(cursor_x, cursor_y, ch, fgcol, bgcol);
        cursor_x += FONT_WIDTH;
        if(cursor_x >= fb_width)
        {
            cursor_x = 0;
            cursor_y += FONT_HEIGHT;
            if(cursor_y >= fb_height)
            {
                gfx_clear(bgcol);
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
            gfx_clear(bgcol);
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
    gfx_clear(VGA_RGBPACK(0, 0, 0));
    cursor_y = 0;
    cursor_x = 0;
    fgcol = VGA_RGBPACK(0xff, 0xff, 0xff);
    bgcol = VGA_RGBPACK(0, 0, 0);
    set_putchar(gfx_putchar);
    set_puts(gfx_puts);

    return true;
}
