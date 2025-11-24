#ifndef __CHAO_PAREDE__
#define __CHAO_PAREDE__


typedef struct{
    unsigned short width;
    unsigned short height;
    unsigned short pos_x;
    unsigned short pos_y;
} wall;

wall* wall_create(unsigned short width, unsigned short height, unsigned short pos_x, unsigned short pos_y);
void wall_draw(wall* element, float camera_x, float camera_y);

int check_collision_wall(square *player, wall *w);
void wall_destroy(wall* element);

#endif