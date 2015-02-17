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

/*
 * This code draws heavily on
 * Jake Gordon's "How to build a racing game" tutorial at
 *
 * <http://codeincomplete.com/projects/racer/>
 *
 * and
 *
 * Louis Gorenfield's "Lou's Pseudo 3d Page" at
 *
 * <http://www.extentofthejam.com/pseudo/>
 *
 * Thanks!
 */

#include "plugin.h"

#include "xracer.h"
#include "compat.h"
#include "generator.h"
#include "graphics.h"
#include "map.h"
#include "maps.h"
#include "road.h"
#include "sprite.h"
#include "util.h"

#include "fixedpoint.h" /* only used for FOV computation */

const struct plugin_api *rb;

/* can move to audiobuf if too big... */
struct road_segment road[MAX_ROAD_LENGTH];

/* this can be changed to allow for variable-sized maps */
unsigned int road_length=MAX_ROAD_LENGTH;

int camera_height = CAMERA_HEIGHT;

void generate_test_road(void)
{
    gen_reset();
    add_road(road, road_length, 0, road_length, 0, 0);
}

void do_early_init(void)
{
#ifdef HAVE_ADJUSTABLE_CPU_FREQ
    /* GIMME DAT RAW POWAH!!! */
    rb->cpu_boost(true);
#endif

    init_alloc();
}

void do_late_init(void)
{
}

enum plugin_status do_flythrough(void)
{
    struct camera_t camera;
    camera.depth = LCD_WIDTH/CAMERA_TAN;

    camera.pos.x = 0;
    /* y is automatically calculated */
    camera.pos.z = 0;

    //generate_test_road();

    //road_length = load_map(road, MAX_ROAD_LENGTH, loop_map, ARRAYLEN(loop_map));

    //road_length = load_external_map(road, MAX_ROAD_LENGTH, "/output.xrm");

    road_length = -1;


    road_length = MAX_ROAD_LENGTH;

    generate_random_road(road, road_length, HILLS, CURVES);

    do_late_init();

    long last_timestamp = *current_tick;

    while(1)
    {
        camera.pos.z += 512;
        /* loop the track right before going off the "end" */
        camera.pos.z %= (road_length - DRAW_DIST) * SEGMENT_LENGTH;

        render(&camera, road, road_length, camera_height);

        char buf[32];
        int dt = *current_tick - last_timestamp;
        snprintf(buf, sizeof(buf), "DT: %d", dt);
        SET_FOREGROUND(LCD_WHITE);
        PUTSXY(0, 0, buf);

        LCD_UPDATE();

        //rb->sleep((HZ/100)-dt);
        YIELD();
        last_timestamp = *current_tick;
    }
}

/* plugin_start(): plugin entry point */
/* contains main loop */
enum plugin_status xracer_main(const struct plugin_api *param)
{
    rb = param;

    do_early_init();

    enum plugin_status rc = do_flythrough();
#ifdef HAVE_ADJUSTABLE_CPU_FREQ
    rb->cpu_boost(false);
#endif
    return rc;
}
