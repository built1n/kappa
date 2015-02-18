#include <stdlib.h>
#include "plugin.h"
#include "gfx.h"
#include "ps2kbd.h"

static void plugin_clear(void)
{
    gfx_clear();
}

static void plugin_drawpix(int x, int y)
{
    gfx_drawpixel(x, y);
}

static void plugin_vline(int a, int b, int c)
{
    gfx_vline(a, b, c);
}

static void plugin_hline(int a, int b, int c)
{
    gfx_hline(a, b, c);
}

static int button_get(void)
{
    return ps2kbd_button_get();
}

static const struct plugin_api kappa_api = {
    &plugin_clear,
    &plugin_hline,
    &plugin_vline,
    &plugin_drawpix,
    &gfx_drawline,
    &gfx_drawrect,
    &gfx_fillrect,
    &gfx_set_foreground,
    &gfx_get_foreground,
    &gfx_set_background,
    &gfx_get_background,
    &srand,
    &rand,
    &gfx_filltriangle,
    &gfx_drawcircle,
    &gfx_fillcircle,
    &gfx_update,
    &gfx_putsxy,
    &button_get
};

void plugin_load(enum plugin_status (*plugin)(const struct plugin_api*))
{
    bool status = gfx_get_doublebuffer();
    gfx_set_doublebuffer(true);
    plugin(&kappa_api);
    gfx_set_doublebuffer(status);
}
