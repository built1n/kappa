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
uint16_t fb_stride;
const uint8_t *gfx_bpp = &fb_bpp;

const uint16_t *gfx_width = &fb_width;
const uint16_t *gfx_height = &fb_height;

static int cursor_x, cursor_y;
uint32_t _gfx_fgcol, _gfx_bgcol;

extern void gfx_clear_packed(void);

void (*gfx_clear)(void) = &gfx_clear_packed;

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

void gfx_clear_unpacked(void)
{
    uint8_t *fb = framebuffer;
    const uint32_t bg = _gfx_bgcol;

    const uint16_t padding = fb_stride - (fb_bpp * fb_width);

    for(int y = 0; y < fb_height; ++y)
    {
        for(int x = 0; x < fb_width; ++x)
        {
            *(uint32_t*)fb++ = bg;
        }
        fb += padding;
    }
}

void gfx_reset(void)
{
    _gfx_fgcol = VGA_RGBPACK(0xff, 0xff, 0xff);
    _gfx_bgcol = VGA_RGBPACK(0, 0, 0);
    gfx_clear();
    cursor_y = 0;
    cursor_x = 0;
}

void gfx_drawchar(int x, int y, int c)
{
    uint8_t *line_addr = framebuffer + (x * fb_bpp) + (y * fb_stride);
    const uint32_t fg = _gfx_fgcol;
    const uint16_t stride = fb_stride;
    const uint8_t stop_y = MIN(FONT_HEIGHT, fb_height - y);
    const uint8_t stop_x = MIN(FONT_WIDTH, fb_width - x);
    if(c < 0 || c > 132)
        return;
    for(int i = 0; i < stop_y; ++i)
    {
        uint8_t mask_table[8] = {128, 64, 32, 16, 8, 4, 2, 1};
        for(int j = 0; j < stop_x; ++j)
        {
            if(gfx_font[c][i] & mask_table[j])
                ((uint32_t*)line_addr)[j] = fg;
        }
        line_addr += stride;
    }
}

void gfx_drawchar_bg(int x, int y, int c)
{
    uint8_t *line_addr = framebuffer + (x * fb_bpp) + (y * fb_stride);
    const uint32_t fg = _gfx_fgcol;
    const uint16_t stride = fb_stride;
    const uint8_t stop_y = MIN(FONT_HEIGHT, fb_height - y);
    const uint8_t stop_x = MIN(FONT_WIDTH, fb_width - x);
    if(c < 0 || c > 132)
        return;
    for(int i = 0; i < stop_y; ++i)
    {
        uint8_t mask_table[8] = {128, 64, 32, 16, 8, 4, 2, 1};
        for(int j = 0; j < stop_x; ++j)
        {
            if(gfx_font[c][i] & mask_table[j])
                ((uint32_t*)line_addr)[j] = fg;
            else
                ((uint32_t*)line_addr)[j] = _gfx_bgcol;
        }
        line_addr += stride;
    }
}

void gfx_putchar(int ch)
{
    if(ch != '\n' && ch != '\b')
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
    else if(ch == '\n')
    {
        cursor_x = 0;
        cursor_y += FONT_HEIGHT;
        if(cursor_y >= fb_height)
        {
            gfx_clear();
            cursor_y = 0;
        }
    }
    else if(ch == '\b')
    {
        int temp_x = cursor_x - FONT_WIDTH;
        if(temp_x < 0)
        {
            cursor_x = 0;
            int temp_y = cursor_y - FONT_HEIGHT;
            cursor_y = (temp_y < 0) ? 0 : temp_y;
        }
        else
        {
            cursor_x = temp_x;
        }
        gfx_drawchar_bg(cursor_x, cursor_y, ' ');
    }
}

void gfx_puts(const char* str)
{
    while(*str)
    {
        gfx_putchar(*str++);
    }
}

/* implemented in assembly now */
#if 0
void gfx_hline(int x1, int x2, int y)
{
    /* make sure x1 is to the left of x2 */
    if(x2 < x1)
    {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }

    uint8_t *base = framebuffer + y * fb_stride;

    uint8_t *dest = base + x1 * fb_bpp;
    uint8_t *stop = base + x2 * fb_bpp;
    const uint32_t col = _gfx_fgcol;
    while(dest < stop)
    {
        *(uint32_t*)dest = col;
        dest += fb_bpp;
    }
}

void gfx_vline(int y1, int y2, int x)
{
    /* make sure y1 is above y2 */
    if(y2 < y1)
    {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    uint8_t *dest = framebuffer + y1 * fb_stride + x * fb_bpp;
    uint8_t *stop = framebuffer + y2 * fb_stride + x * fb_bpp;
    const uint32_t col = _gfx_fgcol;
    const uint16_t stride = fb_stride;
    while(dest < stop)
    {
        *(uint32_t*)dest = col;
        dest += stride;
    }
}
#endif

void gfx_fillrect(int x, int y, int w, int h)
{
    for(int i = 0; i < h; ++i)
    {
        gfx_hline(x, x + w, y + i);
    }
}

void gfx_drawline(int x1, int y1, int x2, int y2)
{
    int dx =  abs(x2 - x1);
    int sx =  x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1);
    int sy =  y1 < y2 ? 1 : -1;
    int err = dx + dy;
    int e2; /* error value e_xy */

    while(1)
    {
        gfx_drawpixel(x1, y1);
        if (x1 == x2 && y1 == y2)
            break;
        e2 = err << 1;
        if (e2 >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

bool gfx_init(struct vbe_info_t *vbe_mode_info)
{
    framebuffer = (uint8_t*)vbe_mode_info->physbase;
    fb_width = vbe_mode_info->Xres;
    fb_height = vbe_mode_info->Yres;
    fb_bpp = vbe_mode_info->bpp / 8;
    fb_stride = vbe_mode_info->pitch;
    if(fb_bpp != 4)
    {
        printf("WARNING: BPP != 32, falling back to text mode...\n");
        return false;
    }

    if(fb_stride != fb_bpp * fb_width)
    {
        printf("Pitch != stride * BPP, fill performance might be suboptimal!\n");
        gfx_clear = &gfx_clear_unpacked;
    }
    else
    {
        gfx_clear = &gfx_clear_packed;
    }

    gfx_reset();
    set_putchar(gfx_putchar);
    set_puts(gfx_puts);

    return true;
}
