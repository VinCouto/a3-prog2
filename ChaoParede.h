#ifndef __CHAO_PAREDE__
#define __CHAO_PAREDE__


typedef struct{
    unsigned short width;
    unsigned short height;
    unsigned short pos_x;
    unsigned short pos_y;
} wall;

wall* wall_create(unsigned short width, unsigned short height, unsigned short pos_x, unsigned short pos_y);
void wall_destroy(wall* element);

#endif