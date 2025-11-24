#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "Square.h"
#include "ChaoParede.h"

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


// Função para verificar colisão entre PLAYER (Square) e PAREDE (Wall)
int check_collision_wall(square *player, wall *w) {
    
    // 1. Definir as bordas do Player (usando heigth com th do seu código)
    int p_esq   = player->x - player->width/2;
    int p_dir   = player->x + player->width/2;
    int p_cima  = player->y - player->heigth/2;
    int p_baixo = player->y + player->heigth/2;

    // 2. Definir as bordas da Parede (usando height correto do ChaoParede.c)
    int w_esq   = w->pos_x - w->width/2;
    int w_dir   = w->pos_x + w->width/2;
    int w_cima  = w->pos_y - w->height/2;
    int w_baixo = w->pos_y + w->height/2;

    // 3. Lógica AABB (Axis-Aligned Bounding Box)
    // Se todas as condições forem verdadeiras, existe sobreposição (colisão)
    if (p_dir > w_esq &&    // Player cruza borda esquerda da parede
        p_esq < w_dir &&    // Player cruza borda direita da parede
        p_baixo > w_cima && // Player cruza topo da parede
        p_cima < w_baixo) { // Player cruza base da parede
        
        return 1; // Colidiu!
    }

    return 0; // Não colidiu
}


void wall_destroy(wall* element){
    free(element);
}