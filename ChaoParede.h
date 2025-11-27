#ifndef __CHAO_PAREDE__
#define __CHAO_PAREDE__


#include "Square.h"
#include <allegro5/allegro5.h>

struct wall {
    unsigned short width;
    unsigned short height;
    unsigned short pos_x;
    unsigned short pos_y;
    ALLEGRO_BITMAP *sprite;
};

wall* wall_create(unsigned short width, unsigned short height, unsigned short pos_x, unsigned short pos_y);
void wall_draw(wall* element, float camera_x, float camera_y);

int check_collision_wall(square *player, wall *w);
int check_hitbox_vs_wall(square *p, Hitbox box, wall *w);
int check_collision_with_map(square *p, Hitbox box, wall **walls, int num_walls);


void wall_destroy(wall* element);

#endif