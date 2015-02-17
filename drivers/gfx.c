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

uint8_t *real_framebuffer = NULL;
uint8_t *temp_framebuffer = NULL;
uint8_t *framebuffer = NULL;
bool double_buffer = false;
uint16_t fb_width;
uint16_t fb_height;

/* this is BYTES per pixel */
uint8_t  fb_bpp;
uint16_t fb_stride;
/* fb_stride / 4 */
uint16_t fb_stride32;
const uint8_t *gfx_bpp = &fb_bpp;

const uint16_t *gfx_width = &fb_width;
const uint16_t *gfx_height = &fb_height;

static int cursor_x, cursor_y;
uint32_t _gfx_fgcol, _gfx_bgcol;

void (*gfx_clear)(void);
void (*gfx_drawpixel)(int x, int y);
void (*gfx_hline)(int x1, int x2, int y);
void (*gfx_vline)(int y1, int y2, int x);

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

/* assembly */
void gfx_drawpixel_32bpp_checked(int x, int y)
{
    if(0 <= x && x < fb_width &&
       0 <= y && y < fb_height)
        ((uint32_t*)framebuffer)[y * fb_stride32 + x] = _gfx_fgcol;
    else
        panic("pixel OOB!\n");
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
void gfx_hline_checked(int x1, int x2, int y)
{
    /* make sure x1 is to the left of x2 */
    if(x2 < x1)
    {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }

    x1 = MAX(0, x1);
    x2 = MIN(x2, fb_width);

    if(0 <= y && y < fb_height)
    {

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
}

void gfx_vline_checked(int y1, int y2, int x)
{
    /* make sure y1 is above y2 */
    if(y2 < y1)
    {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    y1 = MAX(0, y1);
    y2 = MIN(y2, fb_height);

    if(0 <= x && x < fb_width)
    {
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
}

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

void gfx_drawcircle(int cx, int cy, int r)
{
    int d = 3 - (r * 2);
    int x = 0;
    int y = r;
    while(x <= y)
    {
        gfx_drawpixel(cx + x, cy + y);
        gfx_drawpixel(cx - x, cy + y);
        gfx_drawpixel(cx + x, cy - y);
        gfx_drawpixel(cx - x, cy - y);
        gfx_drawpixel(cx + y, cy + x);
        gfx_drawpixel(cx - y, cy + x);
        gfx_drawpixel(cx + y, cy - x);
        gfx_drawpixel(cx - y, cy - x);
        if(d < 0)
        {
            d += (x * 4) + 6;
        }
        else
        {
            d += ((x - y) * 4) + 10;
            --y;
        }
        ++x;
    }
}

void gfx_fillcircle(int cx, int cy, int r)
{
    int d = 3 - (r * 2);
    int x = 0;
    int y = r;
    while(x <= y)
    {
        gfx_hline(cx - x, cx + x, cy + y);
        gfx_hline(cx - x, cx + x, cy - y);
        gfx_hline(cx - y, cx + y, cy + x);
        gfx_hline(cx - y, cx + y, cy - x);
        if(d < 0)
        {
            d += (x * 4) + 6;
        }
        else
        {
            d += ((x - y) * 4) + 10;
            --y;
        }
        ++x;
    }
}

/* these next two functions were taken directly from the Rockbox project's XLCD
 * library.
 * Copyright (C) 2005 Jens Arnold */

/* sort the given coordinates by increasing x value */
static void sort_points_by_increasing_y(int* y1, int* x1,
                                        int* y2, int* x2,
                                        int* y3, int* x3)
{
    int x, y;
    if (*x1 > *x3)
    {
        if (*x2 < *x3)       /* x2 < x3 < x1 */
        {
            x = *x1; *x1 = *x2; *x2 = *x3; *x3 = x;
            y = *y1; *y1 = *y2; *y2 = *y3; *y3 = y;
        }
        else if (*x2 > *x1)  /* x3 < x1 < x2 */
        {
            x = *x1; *x1 = *x3; *x3 = *x2; *x2 = x;
            y = *y1; *y1 = *y3; *y3 = *y2; *y2 = y;
        }
        else               /* x3 <= x2 <= x1 */
        {
            x = *x1; *x1 = *x3; *x3 = x;
            y = *y1; *y1 = *y3; *y3 = y;
        }
    }
    else
    {
        if (*x2 < *x1)       /* x2 < x1 <= x3 */
        {
            x = *x1; *x1 = *x2; *x2 = x;
            y = *y1; *y1 = *y2; *y2 = y;
        }
        else if (*x2 > *x3)  /* x1 <= x3 < x2 */
        {
            x = *x2; *x2 = *x3; *x3 = x;
            y = *y2; *y2 = *y3; *y3 = y;
        }
        /* else already sorted */
    }
}

/* draw a filled triangle, using horizontal lines for speed */
void gfx_filltriangle(int x1, int y1,
                      int x2, int y2,
                      int x3, int y3)
{
    long fp_x1, fp_x2, fp_dx1, fp_dx2;
    int y;
    sort_points_by_increasing_y(&x1, &y1, &x2, &y2, &x3, &y3);

    if (y1 < y3)  /* draw */
    {
        fp_dx1 = ((x3 - x1) << 16) / (y3 - y1);
        fp_x1  = (x1 << 16) + (1<<15) + (fp_dx1 >> 1);

        if (y1 < y2)  /* first part */
        {
            fp_dx2 = ((x2 - x1) << 16) / (y2 - y1);
            fp_x2  = (x1 << 16) + (1<<15) + (fp_dx2 >> 1);
            for (y = y1; y < y2; y++)
            {
                gfx_hline(fp_x1 >> 16, fp_x2 >> 16, y);
                fp_x1 += fp_dx1;
                fp_x2 += fp_dx2;
            }
        }
        if (y2 < y3)  /* second part */
        {
            fp_dx2 = ((x3 - x2) << 16) / (y3 - y2);
            fp_x2 = (x2 << 16) + (1<<15) + (fp_dx2 >> 1);
            for (y = y2; y < y3; y++)
            {
                gfx_hline(fp_x1 >> 16, fp_x2 >> 16, y);
                fp_x1 += fp_dx1;
                fp_x2 += fp_dx2;
            }
        }
    }
}

static void gfx_bitmap32(int x, int y, const struct bitmap_t *bmp)
{
    /* SLOOW */
    uint8_t *data = bmp->data;
    for(unsigned int i = y; i < y + bmp->h && i < fb_height; ++i)
    {
        for(unsigned int j = x; j < x + bmp->w && j < fb_width; ++j)
        {
            uint8_t r = *data++;
            uint8_t g = *data++;
            uint8_t b = *data++;
            gfx_set_foreground(VGA_RGBPACK(r, g, b));
            gfx_drawpixel(j, i);
        }
    }
}

void gfx_bitmap(int x, int y, const struct bitmap_t *bmp)
{
    gfx_bitmap32(x, y, bmp);
}

void gfx_drawrect(int x, int y, int w, int h)
{
    gfx_hline(MAX(0, x), MIN(x + w, fb_width), MAX(0, y));
    gfx_hline(MAX(0, x), MIN(x + w, fb_width), MIN(y + h, fb_height));
    gfx_vline(MAX(0, y), MIN(y + h, fb_height),MAX(0, x));
    gfx_vline(MAX(0, y), MIN(y + h, fb_height),MIN(x + w, fb_width));
}

void gfx_update(void)
{
    memcpy(real_framebuffer, framebuffer, fb_height * fb_stride);
}

void gfx_set_doublebuffer(bool yesno)
{
    if(yesno)
        framebuffer = temp_framebuffer;
    else
        framebuffer = real_framebuffer;
}

bool gfx_get_doublebuffer(void)
{
    if(framebuffer == temp_framebuffer)
        return true;
    else
        return false;
}

void gfx_putsxy(int x, int y, const char* str)
{
    while(*str)
    {
        gfx_drawchar(x, y, *str);
        x += FONT_WIDTH;
        str++;
    }
}

void gfx_putsxy_bg(int x, int y, const char* str)
{
    while(*str)
    {
        gfx_drawchar_bg(x, y, *str);
        x += FONT_WIDTH;
        str++;
    }
}

bool gfx_init(struct vbe_info_t *vbe_mode_info)
{
    real_framebuffer = (uint8_t*)vbe_mode_info->physbase;
    gfx_set_doublebuffer(false);
    fb_width = vbe_mode_info->Xres;
    fb_height = vbe_mode_info->Yres;
    fb_bpp = vbe_mode_info->bpp / 8;
    fb_stride = vbe_mode_info->pitch;
    fb_stride32 = fb_stride / sizeof(uint32_t);
    gfx_hline = gfx_hline_checked;
    gfx_vline = gfx_vline_checked;
    temp_framebuffer = malloc(fb_height * fb_stride);
    if(fb_bpp != 4)
    {
        printf("WARNING: BPP != 32, falling back to text mode...\n");
        return false;
    }
    else
    {
        extern void gfx_drawpixel_32bpp(int, int);
        gfx_drawpixel = &gfx_drawpixel_32bpp;
    }

    set_putchar(gfx_putchar);
    set_puts(gfx_puts);

    /* A bit of fragile code here... don't call gfx_reset() before setting gfx_clear! */

    if(fb_stride != fb_bpp * fb_width)
    {
        gfx_clear = &gfx_clear_unpacked;
        gfx_reset();
        printf("WARNING: Internal framebuffer padding detected, support is experimental.\n");
    }
    else
    {
        /* assembly */
        extern void gfx_clear_packed(void);
        gfx_clear = &gfx_clear_packed;
        gfx_reset();
    }

    return true;
}
