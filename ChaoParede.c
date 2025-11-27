#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdlib.h>

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
    new_wall->sprite = NULL; // Inicializa o sprite como NULL
    return new_wall;
} 

void wall_draw(wall* element, float camera_x, float camera_y){
    float wall_screen_x = element->pos_x - camera_x;
    float wall_screen_y = element->pos_y - camera_y;

    al_draw_filled_rectangle(
        wall_screen_x - element->width/2, 
        wall_screen_y - element->height/2, 
        wall_screen_x + element->width/2, 
        wall_screen_y + element->height/2, 
        al_map_rgb(255, 255, 255)
    );
    printf("desenhando parede em x: %.2f y: %.2f\n", wall_screen_x, wall_screen_y);
}

// Função para verificar colisão entre PLAYER (Square) e PAREDE (Wall)
int check_collision_wall(square *player, wall *w) {
    
    float p_esq   = player->x - player->width/2.0f;
    float p_dir   = player->x + player->width/2.0f;
    float p_cima  = player->y - player->heigth/2.0f;
    float p_baixo = player->y + player->heigth/2.0f;

    float w_esq   = w->pos_x - w->width/2.0f;
    float w_dir   = w->pos_x + w->width/2.0f;
    float w_cima  = w->pos_y - w->height/2.0f;
    float w_baixo = w->pos_y + w->height/2.0f;

    // Se todas as condições forem verdadeiras, existe sobreposição (colisão)
    if (p_dir > w_esq &&    // Player cruza borda esquerda da parede
        p_esq < w_dir &&    // Player cruza borda direita da parede
        p_baixo > w_cima && // Player cruza topo da parede
        p_cima < w_baixo) { // Player cruza base da parede
        
        return 1; // Colidiu!
    }

    return 0; // Não colidiu
}

int check_hitbox_vs_wall(square *p, Hitbox box, wall *w) {
    if (!box.active) return 0; // Se a hitbox estiver desligada, ignora

    // 1. Calcular bordas da HITBOX no mundo (usar float para precisão)
    float hb_esq   = (p->x + box.offset_x) - box.width / 2.0f;
    float hb_dir   = (p->x + box.offset_x) + box.width / 2.0f;
    float hb_cima  = (p->y + box.offset_y) - box.height / 2.0f;
    float hb_baixo = (p->y + box.offset_y) + box.height / 2.0f;

    // 2. Calcular bordas da PAREDE (usar float para precisão)
    float w_esq   = w->pos_x - w->width / 2.0f;
    float w_dir   = w->pos_x + w->width / 2.0f;
    float w_cima  = w->pos_y - w->height / 2.0f;
    float w_baixo = w->pos_y + w->height / 2.0f;

    // 3. Verificar sobreposição (AABB)
    if (hb_dir > w_esq && 
        hb_esq < w_dir && 
        hb_baixo > w_cima && 
        hb_cima < w_baixo) {
        return 1; // Colidiu
    }

    return 0;
}


int check_collision_with_map(square *p, Hitbox box, wall **walls, int num_walls) {
    
    // Percorre todas as paredes existentes
    for (int i = 0; i < num_walls; i++) {
        if (walls[i] != NULL) {
            // Chama a função individual criada no passo 2
            if (check_hitbox_vs_wall(p, box, walls[i])) {
                return 1; // Bateu em alguém! Para de procurar e avisa.
            }
        }
    }
    
    return 0; // Passou por todas e não bateu em nada.
}

void wall_destroy(wall* element){
    free(element);
}