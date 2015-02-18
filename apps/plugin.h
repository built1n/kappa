#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "gfx.h"
#include "ps2kbd.h"
#include "timer.h"

#define LCD_WIDTH (*gfx_width)
#define LCD_HEIGHT (*gfx_height)
#define LCD_BLACK VGA_RGBPACK(0, 0, 0)
#define LCD_WHITE VGA_RGBPACK(255,255,255)
#define LCD_RGBPACK VGA_RGBPACK
#define LOGF printf
#define ARRAYLEN(a) (sizeof(a)/sizeof(a[0]))

#define RGB_UNPACK_RED(x) ((x & 0xFF0000) >> 16)
#define RGB_UNPACK_GREEN(x) ((x & 0xFF00) >> 8)
#define RGB_UNPACK_BLUE(x) ((x & 0xFF) >> 0)

enum plugin_status {
    PLUGIN_OK = 0, /* PLUGIN_OK == EXIT_SUCCESS */
    /* 1...255 reserved for exit() */
    PLUGIN_POWEROFF,
    PLUGIN_GOTO_WPS,
    PLUGIN_ERROR = -1,
};

struct plugin_api {
    void (*lcd_clear_display)(void);
    void (*lcd_hline)(int x1, int x2, int y);
    void (*lcd_vline)(int x, int y1, int y2);
    void (*lcd_drawpixel)(int x, int y);
    void (*lcd_drawline)(int x1, int y1, int x2, int y2);
    void (*lcd_drawrect)(int x, int y, int width, int height);
    void (*lcd_fillrect)(int x, int y, int width, int height);
    void     (*lcd_set_foreground)(unsigned foreground);
    unsigned (*lcd_get_foreground)(void);
    void     (*lcd_set_background)(unsigned foreground);
    unsigned (*lcd_get_background)(void);

    void (*srand)(unsigned int seed);
    unsigned int (*rand)(void);

    void (*lcd_filltriangle)(int, int, int, int, int, int);
    void (*lcd_drawcircle)(int x, int y, int r);
    void (*lcd_fillcircle)(int x, int y, int r);
    void (*lcd_update)(void);
    void (*lcd_putsxy)(int x, int y, const char*);

    int  (*button_get)(void);
    int  (*modifier_get)(void);
};

/* defined by the plugin */
extern const struct plugin_api *rb;

#endif
