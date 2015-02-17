#include <stdbool.h>
#include <stdint.h>

#include "generator.h"
#include "map.h"
#include "util.h"

static bool add_section(struct road_segment *road, unsigned int max_road_length, unsigned int *back, struct road_section *map)
{
    bool ret = true;
    switch(map->type)
    {
    case 0:
        /* constant segment */
        add_road(road, max_road_length, *back, map->len, map->curve, map->slope);
        *back += map->len;
        break;
    case 1:
        /* up-hill */
        add_uphill(road, max_road_length, *back, map->slope, map->len, map->curve);
        *back += map->len + 2*map->slope;
        break;
    case 2:
        add_downhill(road, max_road_length, *back, map->slope, map->len, map->curve);
        *back += map->len + 2*map->slope;
        break;
    default:
        warning("invalid type id");
        ret = false;
        break;
    }
    return ret;
}

unsigned load_map(struct road_segment *road, unsigned int max_road_length, struct road_section *map, unsigned int map_length)
{
    gen_reset();
    unsigned int back = 0;
    for(unsigned int i=0;i<map_length;++i)
    {
        add_section(road, max_road_length, &back, &map[i]);
    }
    return back;
}
