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
2 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#ifndef _XRACER_H_
#define _XRACER_H_

/* miscellaneous #defines and data types */

#include "plugin.h"

/* colors */
#define SKY_COLOR1 LCD_RGBPACK(0, 0x96, 0xff) /* top of sky gradient */
#define SKY_COLOR2 LCD_RGBPACK(0xfe, 0xfe, 0xfe) /* bottom of sky gradient */
#define GRASS_COLOR1 LCD_RGBPACK(0, 0xc5, 0)
#define GRASS_COLOR2 LCD_RGBPACK(0, 0x9a, 0)
#define ROAD_COLOR1 LCD_RGBPACK(120, 120, 120)
#define ROAD_COLOR2 LCD_RGBPACK(80, 80, 80)
#define BORDER_COLOR1 LCD_RGBPACK(240, 0, 0)
#define BORDER_COLOR2 LCD_RGBPACK(240, 240, 240)
#define LANE_COLOR LCD_RGBPACK(0xcc, 0xcc, 0xcc)

/* road parameters */
#define ROAD_WIDTH (LCD_WIDTH/2) /* actually half the road width for easier math */
#define MAX_ROAD_LENGTH 4096
#define SEGMENT_LENGTH (LCD_WIDTH * 12)

/* road generator parameters */
#define HILLS true
#define CURVES true

/* this specifies the length of each road and border color in segments */
#define RUMBLE_LENGTH 3
#define LANES 3

/* camera parameters */
#define DRAW_DIST 256

/* NOTE: FOV is not used, the tangent value (see below) is */
#define CAMERA_FOV 120 /* in degrees */
#define CAMERA_TAN 1.732050808L
#define CAMERA_HEIGHT 200

/* game parameters */
/* currently this is how far the camera moves per keypress */
#define MANUAL_SPEED 50

/* number of bits to use for the fractional part of fixed-point numbers */
/* note that FOV calculation always uses 14 by default */
#define FRACBITS 8

/* the number of fractional bits to use for high-precision numbers */
#define HIPREC_FRACBITS 24

#endif
