#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "obstacle.h"

obstacle* obstacle_create(float pos_x, float pos_y, unsigned short width, unsigned short height, const char* sprite_path, int damage, float knockback_force){
    obstacle* new_obstacle = (obstacle*) malloc(sizeof(obstacle));
    if (!new_obstacle) return NULL;
    new_obstacle->pos_x = pos_x;
    new_obstacle->pos_y = pos_y;
    new_obstacle->width = width;
    new_obstacle->height = height;
    new_obstacle->damage = damage;
    new_obstacle->knockback_force = knockback_force;
    new_obstacle->check_touch = 0; // Inicializa a flag como 0 (não tocado)

    if(sprite_path != NULL){
        new_obstacle->sprite = al_load_bitmap(sprite_path);
        if (!new_obstacle->sprite) {
            fprintf(stderr, "Failed to load obstacle sprite from %s\n", sprite_path);
            free(new_obstacle);
            return NULL;
        }
    } else {
        new_obstacle->sprite = NULL; // Sem sprite
    }
    return new_obstacle;
}

int check_collision_obstacle(square* player, obstacle* obs){
    float p_esq   = player->x - player->width/2.0f;
    float p_dir   = player->x + player->width/2.0f;
    float p_cima  = player->y - player->heigth/2.0f;
    float p_baixo = player->y + player->heigth/2.0f;

    float o_esq   = obs->pos_x - obs->width/2.0f;
    float o_dir   = obs->pos_x + obs->width/2.0f;
    float o_cima  = obs->pos_y - obs->height/2.0f;
    float o_baixo = obs->pos_y + obs->height/2.0f;

    // Se todas as condições forem verdadeiras, existe sobreposição (colisão)
    if (p_dir > o_esq &&    // Player cruza borda esquerda do obstáculo
        p_esq < o_dir &&    // Player cruza borda direita do obstáculo
        p_baixo > o_cima && // Player cruza topo do obstáculo
        p_cima < o_baixo) { // Player cruza base do obstáculo
        
        return 1; // Colidiu!
    }

    return 0; // Não colidiu
}


// Nova função para aplicar knockback
void apply_knockback(square* player, obstacle* obs) {
    // Calcula a direção do knockback (longe do obstáculo)
    float dx = player->x - obs->pos_x;
    float dy = player->y - obs->pos_y;
    
    // Calcula a distância
    float distance = sqrt(dx * dx + dy * dy);
    
    // Se a distância for muito pequena, evita divisão por zero
    if (distance < 1.0f) distance = 1.0f;
    
    // Normaliza a direção
    float norm_x = dx / distance;
    float norm_y = dy / distance;
    
    // Aplica o knockback
    player->x += norm_x * obs->knockback_force;
    player->vy -= obs->knockback_force * 0.5f;  // Knockback vertical também
}



void obstacle_destroy(obstacle* element){
  if(element->sprite){
      al_destroy_bitmap(element->sprite);
  }
  free(element);
}