#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "Square.h"
#include "menu.c"
#include "ChaoParede.h"
#include "AggressiveSquares.c"

#define X_SCREEN 920
#define Y_SCREEN 640

wall* wall_create(unsigned short width, unsigned short height, unsigned short pos_x, unsigned short pos_y){
    wall* new_wall = (wall*) malloc(sizeof(wall));
    if (!new_wall) return NULL;
    new_wall->width = width;
    new_wall->height = height;
    new_wall->pos_x = pos_x;
    new_wall->pos_y = pos_y;
    return new_wall;
} 

void wall_destroy(wall* element){
    free(element);
}