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

#include "fixedpoint.h"

#include "xracer.h"
#include "compat.h"
#include "graphics.h"
#include "road.h"
#include "sprite.h"
#include "util.h"

/* fill poly takes 4 screenspace coordinates and fills them */
/* order of arguments matters: */
/* (x1, y1)      (x2, y2) */
/* (x3, y3)      (x4, y4) */
static inline void fill_poly(int x1, int y1,
                             int x2, int y2,
                             int x3, int y3,
                             int x4, int y4,
                             unsigned color)
{
    SET_FOREGROUND(color);

    FILL_TRI(x1, y1, x2, y2, x3, y3);
    FILL_TRI(x1, y1, x3, y3, x4, y4);
}

/* project(): performs 3d projection of pt_3d onto pt_2d given camera parameters */
/* this takes arguments by value so as to allow easy changing of camera positions at render time*/

static inline int project(int cam_x, int cam_y, int cam_z, int camera_depth, struct point_3d *pt_3d, struct point_2d *pt_2d, float *scale_return)
{
    /* calculate the coordinates relative to the camera */
    int rel_x, rel_y, rel_z;
    rel_x = pt_3d->x - cam_x;
    rel_y = pt_3d->y - cam_y;
    rel_z = pt_3d->z - cam_z;
    if(rel_z != 0)
    {
        float scale = (float)camera_depth / rel_z;
        pt_2d->x = (LCD_WIDTH/2) + (scale * rel_x * (LCD_WIDTH/2));
        pt_2d->y = (LCD_WIDTH/2) - (scale * rel_y * (LCD_HEIGHT/2));
        pt_2d->w = scale * ROAD_WIDTH * (LCD_WIDTH/2);
        if(scale_return)
            *scale_return=scale;

        return rel_z;
    }
    return -1;
}

static inline int border_width(int projected_road_width, int lanes)
{
    return projected_road_width/MAX(6, 2*lanes);
}

static inline int lane_marker_width(int projected_road_width, int lanes)
{
    return projected_road_width/MAX(32, 8*lanes);
}

/* fill_gradient(): draws a gradient across the whole screen from top to bottom,
   changing from top_color to bottom_color */

static void fill_gradient(int top, unsigned top_color, int bottom, unsigned bottom_color)
{
    long top_r, top_g, top_b;
    top_r = RGB_UNPACK_RED(top_color)   << FRACBITS;
    top_g = RGB_UNPACK_GREEN(top_color) << FRACBITS;
    top_b = RGB_UNPACK_BLUE(top_color)  << FRACBITS;

    long bottom_r, bottom_g, bottom_b;
    bottom_r = RGB_UNPACK_RED(bottom_color)   << FRACBITS;
    bottom_g = RGB_UNPACK_GREEN(bottom_color) << FRACBITS;
    bottom_b = RGB_UNPACK_BLUE(bottom_color)  << FRACBITS;

    bottom <<= FRACBITS;
    top    <<= FRACBITS;
    long r_step = fp_div((bottom_r-top_r), (bottom-top), FRACBITS);
    long g_step = fp_div((bottom_g-top_g), (bottom-top), FRACBITS);
    long b_step = fp_div((bottom_b-top_b), (bottom-top), FRACBITS);

    long r = top_r;
    long g = top_g;
    long b = top_b;

    bottom >>= FRACBITS;
    top    >>= FRACBITS;

    for(int i = top; i<bottom; ++i)
    {
        SET_FOREGROUND(LCD_RGBPACK( ROUND(r), ROUND(g), ROUND(b) ));
        DRAW_HLINE(0, LCD_WIDTH, i);
        r += r_step;
        g += g_step;
        b += b_step;
    }
}

/* draws a segment on screen given its 2d screen coordinates and other data */

static inline void render_segment(struct point_2d *p1, struct point_2d *p2,
                                  unsigned road_color, unsigned border_color,
                                  int lanes, unsigned lane_color, unsigned grass_color)
{
    int border_1 = border_width(p1->w, lanes);
    int border_2 = border_width(p2->w, lanes);

    /* draw grass first */
    SET_FOREGROUND(grass_color);
    FILL_RECT(0, p2->y, LCD_WIDTH, p1->y - p2->y);

    /* draw borders */
    fill_poly(p1->x-p1->w-border_1, p1->y, p1->x-p1->w, p1->y,
              p2->x-p2->w, p2->y, p2->x-p2->w-border_2, p2->y, border_color);

    fill_poly(p1->x+p1->w+border_1, p1->y, p1->x+p1->w, p1->y,
              p2->x+p2->w, p2->y, p2->x+p2->w+border_2, p2->y, border_color);

    /* draw road */
    fill_poly(p2->x-p2->w, p2->y, p2->x+p2->w, p2->y, p1->x+p1->w, p1->y, p1->x-p1->w, p1->y, road_color);

    /* draw lanes */
    if(lanes > 1)
    {
        int lane_1 = lane_marker_width(p1->w, lanes);
        int lane_2 = lane_marker_width(p2->w, lanes);

        int lane_w1, lane_w2, lane_x1, lane_x2;
        lane_w1 = p1->w*2/lanes;
        lane_w2 = p2->w*2/lanes;
        lane_x1 = p1->x - p1->w + lane_w1;
        lane_x2 = p2->x - p2->w + lane_w2;
        for(int i=1; i<lanes; lane_x1 += lane_w1, lane_x2 += lane_w2, ++i)
        {
            fill_poly(lane_x1 - lane_1/2, p1->y, lane_x1 + lane_1/2, p1->y,
                      lane_x2 + lane_2/2, p2->y, lane_x2 - lane_2/2, p2->y, lane_color);
        }
    }
}

/* project_segment(): does the 3d calculations on a road segment */

static inline bool project_segment(struct road_segment *base, struct road_segment *seg, int x, int dx,
                                   struct camera_t *camera, struct point_2d *p1, struct point_2d *p2, int *maxy,
                                   int road_length)
{
    bool looped = (seg->idx < base->idx);
    seg->clip = *maxy;

    /* future optimization: do one projection per segment because segment n's p2 is the same as segments n+1's p1*/

    /* input points (3d space) */

    struct point_3d p1_3d={-x,    seg->p1_y, seg->p1_z};
    struct point_3d p2_3d={-x-dx, seg->p2_y, seg->p2_z};

    int camera_offset = (looped ? road_length * SEGMENT_LENGTH - camera->pos.z : 0);

    /* 3d project the 2nd point FIRST because its results can be used to save some work */

    int p2_cz = project(camera->pos.x, camera->pos.y, camera->pos.z - camera_offset, camera->depth, &p2_3d, p2, NULL);

    /* decide whether or not to draw this segment */

    if(p2_cz <= camera->depth || /* behind camera */
       p2->y >= *maxy)           /* clipped */
    {
        return false;
    }

    /* nothing is drawn below this */
    *maxy = p2->y;

    /* now 3d project the first point */

    /* save the scale factor to draw sprites */
    project(camera->pos.x, camera->pos.y, camera->pos.z - camera_offset, camera->depth, &p1_3d, p1, &seg->p1_scale);

    if(p2->y >= p1->y) /* backface cull */
    {
        return false;
    }
    return true;
}

/* render(): The primary render driver function
 *
 * calculates segment coordinates and calls render_segment to draw the segment on-screen
 * also handles the faking of curves
 */

void render(struct camera_t *camera, struct road_segment *road, unsigned int road_length, int camera_height)
{
    fill_gradient(0, SKY_COLOR1, LCD_HEIGHT, SKY_COLOR2);

    struct road_segment *base = FIND_SEGMENT(camera->pos.z, road, road_length);

    long base_percent = fp_div((camera->pos.z % SEGMENT_LENGTH) << FRACBITS, SEGMENT_LENGTH << FRACBITS, FRACBITS);
    long dx = - fp_mul(base->curve, base_percent, FRACBITS);
    long x  = 0;

    /* clipping height, nothing is drawn below this */
    int maxy = LCD_HEIGHT;

    /* move the camera to a fixed point above the road */
    /* interpolate so as to prevent jumpy camera movement on hills */
    camera->pos.y = INTERPOLATE(base->p1_y, base->p2_y, base_percent) + camera_height;

    for(int i = 0; i < camera->draw_dist; ++i)
    {
        struct road_segment *seg = &road[(base->idx + i) % road_length];

        if(project_segment(base, seg, ROUND(x), ROUND(dx), camera, &seg->p1, &seg->p2, &maxy, road_length))
        {
            render_segment(&seg->p1, &seg->p2, seg->color, seg->border_color, seg->lanes, seg->lane_color, seg->grass_color);
        }
        /* curve calculation */
        x  += dx;
        dx += seg->curve; /* seg->curve is already FP */
    }
}
