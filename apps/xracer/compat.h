/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2015 Franklin Wei
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#include "plugin.h"

/* this file contains some #defines to make this as platform-neutral as possible */

/* rockbox/kappa API */

#define FILL_TRI(x1, y1, x2, y2, x3, y3) rb->lcd_filltriangle(x1, y1, x2, y2, x3, y3)
#define FILL_RECT(x, y, w, h) rb->lcd_fillrect(x, y, w, h)
#define DRAW_HLINE(x1, x2, y) rb->lcd_hline(x1, x2, y)
#define CLEAR_DISPLAY() rb->lcd_clear_display()
#define SET_FOREGROUND(x) rb->lcd_set_foreground(x)
#define SET_BACKGROUND(x) rb->lcd_set_background(x)
#define LCD_UPDATE() rb->lcd_update()
#define PUTSXY(x, y, s) rb->lcd_putsxy(x, y, s)
#define YIELD()
#define LOGF printf
