//Compilação: gcc AggressiveSquares.c Square.c Joystick.c Bullet.c Pistol.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>																																									//Biblioteca de figuras básicas
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "mapa.h"																																												//Inclusão da biblioteca de quadrados
#include "ChaoParede.h"
#include "menu.h"	
#include "obstacle.h"

#define X_SCREEN 920																																														//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 640																																														//Definição do tamanho da tela em pixels no eixo y

#define MAX_WALLS 30
#define MAX_OBSTACLES 20

#define X_BACKGROUND 4800																																													//Definição do tamanho da tela em pixels no eixo x
#define Y_BACKGROUND 1280																																													//Definição do tamanho da tela em


wall* check_collision_map(square *p, wall **walls, int num_walls) {
    for (int i = 0; i < num_walls; i++) {
        if (walls[i] != NULL) {
            if (check_collision_wall(p, walls[i])) {
								printf("colidiu com parede %d\n", i);
                return walls[i]; // Retorna a parede específica que causou colisão
            }
        }
    }
    return NULL; 
}


// Verifica colisão apenas no eixo X (paredes laterais)
int check_collision_horizontal(square *p, wall **walls, int num_walls) {
    for (int i = 0; i < num_walls; i++) {
        if (walls[i] != NULL) {
            float p_esq   = p->x - p->width/2.0f;
            float p_dir   = p->x + p->width/2.0f;
            float p_cima  = p->y - p->heigth/2.0f;
            float p_baixo = p->y + p->heigth/2.0f;

            float w_esq   = walls[i]->pos_x - walls[i]->width/2.0f;
            float w_dir   = walls[i]->pos_x + walls[i]->width/2.0f;
            float w_cima  = walls[i]->pos_y - walls[i]->height/2.0f;
            float w_baixo = walls[i]->pos_y + walls[i]->height/2.0f;

            // Colisão apenas horizontal: player bate nas laterais (esq ou dir) da parede
            // Ignora colisão se player está apenas tocando o topo (chão)
            if (p_dir > w_esq && p_esq < w_dir && p_baixo > w_cima && p_cima < w_baixo) {
                // Checa se a colisão é lateral (esquerda ou direita)
                // Overlap à esquerda: player saiu pela esquerda e bateu
                float left_overlap = p_dir - w_esq;
                // Overlap à direita: player saiu pela direita e bateu
                float right_overlap = w_dir - p_esq;
                // Overlap no topo: player descer e bateu no topo (chão)
                float top_overlap = p_baixo - w_cima;
                // Overlap no fundo: player subir e bateu no fundo (teto)
                float bottom_overlap = w_baixo - p_cima;

                // Se o menor overlap é lateral (não no topo), há colisão horizontal
                float min_overlap = left_overlap;
                if (right_overlap < min_overlap) min_overlap = right_overlap;
                
                // Se o top_overlap é o menor (player vindo de cima), ignora
                // Só detecta colisão se vier dos lados
                if ((left_overlap < right_overlap && left_overlap < top_overlap && left_overlap <= bottom_overlap) ||
                    (right_overlap < left_overlap && right_overlap < top_overlap && right_overlap <= bottom_overlap)) {
                    return 1; // Colisão lateral detectada
                }
            }
        }
    }
    return 0;
}

void update_life(square* victim, ALLEGRO_FONT* font, ALLEGRO_BITMAP* background){
	if(victim->hp == 5){
		al_draw_text(font, al_map_rgb(255,255,255), 100, 75, 0, "HP = 5");																				//Se o segundo jogador morreu, declare o primeiro jogador vencedor
	} else {	
		if(victim->hp == 4){
			al_draw_text(font, al_map_rgb(255,255,255), 100, 75, 0, "HP = 4");
		} else {
			if(victim->hp == 3){
				al_draw_text(font, al_map_rgb(255,255,255), 100, 75, 0, "HP = 3");
			} else {
				if(victim->hp == 2){
					al_draw_text(font, al_map_rgb(255,255,255), 100, 75, 0, "HP = 2");
				} else {
					if(victim->hp == 1){
						al_draw_text(font, al_map_rgb(255,255,255), 100, 75, 0, "HP = 1");
					}
				}
			}
		}
	}
}


int check_hitbox_collision(square *p1, Hitbox box1, square *p2, Hitbox box2) {
    if (!box1.active || !box2.active) return 0;

    // Calcular posição absoluta no mundo
    int p1_left   = (p1->x + box1.offset_x) - box1.width / 2;
    int p1_right  = (p1->x + box1.offset_x) + box1.width / 2;
    int p1_top    = (p1->y + box1.offset_y) - box1.height / 2;
    int p1_bottom = (p1->y + box1.offset_y) + box1.height / 2;

    int p2_left   = (p2->x + box2.offset_x) - box2.width / 2;
    int p2_right  = (p2->x + box2.offset_x) + box2.width / 2;
    int p2_top    = (p2->y + box2.offset_y) - box2.height / 2;
    int p2_bottom = (p2->y + box2.offset_y) + box2.height / 2;

    // Lógica AABB Padrão
    if (p1_right > p2_left && 
        p1_left < p2_right && 
        p1_bottom > p2_top && 
        p1_top < p2_bottom) {
        return 1;
    }
    return 0;
}


unsigned char collision_2D(square *element_first, square *element_second){																																	//Implementação da função de verificação de colisão entre dois quadrados

	if ((((element_second->y-element_second->heigth/2 >= element_first->y-element_first->heigth/2) && (element_first->y+element_first->heigth/2 >= element_second->y-element_second->heigth/2)) || 	//				//Verifica se o primeiro elemento colidiu com o segundo no eixo X 
		((element_first->y-element_first->heigth/2 >= element_second->y-element_second->heigth/2) && (element_second->y+element_second->heigth/2 >= element_first->y-element_first->heigth/2))) && 	//				//Verifica se o segundo elemento colidiu com o primeiro no eixo X 
		(((element_second->x-element_second->width/2 >= element_first->x-element_first->width/2) && (element_first->x+element_first->width/2 >= element_second->x-element_second->width/2)) || 	//				//Verifica se o primeiro elemento colidiu com o segundo no eixo Y
		((element_first->x-element_first->width/2 >= element_second->x-element_second->width/2) && (element_second->x+element_second->width/2 >= element_first->x-element_first->width/2)))) return 1;			//Verifica se o segundo elemento colidiu com o primeiro no eixo Y
	else return 0;																																															//Se as condições não forem satisfeita, não houve colisão
}

unsigned char check_kill(square *victim){																																					//Implementação da função que verifica se um projétil acertou um jogador

	if(victim->hp <= 0)																																														//Verifica se a vida do jogador vítima é menor ou igual a zero
		return 1;																																																//Se sim, retorne verdadeiro
	else	
		return 0;																																																//Se não houver nenhum projétil que acertou a vítima, retorne falso
}


void update_position(square *player, wall **walls, int num_walls) {
    
    // --- EIXO X (MOVIMENTAÇÃO E COLISÃO COM PAREDES) ---

    // 1. Tenta Mover para ESQUERDA
    if (player->control->left) {
        // Move
        square_move(player, 1, 0, X_BACKGROUND, Y_BACKGROUND);
        
        // Verifica se entrou numa parede lateral
        if (check_collision_horizontal(player, walls, num_walls)) {
            // Se bateu, desfaz o movimento (Move para direita)
            square_move(player, 1, 1, X_BACKGROUND, Y_BACKGROUND);
        }
    }

    // 2. Tenta Mover para DIREITA
    if (player->control->right) {
        // Move
        square_move(player, 1, 1, X_BACKGROUND, Y_BACKGROUND);
        
        // Verifica se entrou numa parede lateral
        if (check_collision_horizontal(player, walls, num_walls)) {
            // Se bateu, desfaz o movimento (Move para esquerda)
            square_move(player, 1, 0, X_BACKGROUND, Y_BACKGROUND);
        }
    }

    // --- COMANDOS DE ESTADO (AGACHAR/PULAR) ---

    // 3. Agachar (CTR)
    if (player->control->ctr) {
        square_move(player, 1, 4, X_BACKGROUND, Y_BACKGROUND);
    }

    // 4. Pular (CIMA) - Só pula se NÃO estiver no ar (idle != 2)
    if (player->control->up && player->idle != 2) {
        
        // Se estava agachado, levanta antes de pular
        if (player->idle == 1) {
            player->heigth = player->heigth * 2; 
            player->y = player->y - player->heigth/4; 
            player->idle = 0;
        }

        // Aplica a força do pulo
        player->vy = FORCA_PULO; 
        player->idle = 2; // Estado: No Ar
    }
}



void update_physics(square *player, wall **walls, int num_walls) {
    
    // 1. Aplica Gravidade
    player->vy += GRAVIDADE;
    
    // Limite de velocidade de queda (Terminal Velocity) para não atravessar o chão
    if (player->vy > 20.0f) player->vy = 20.0f;

	// 2. Move Y (usar float para não truncar incremendtos fracionários)
	player->y += player->vy;

    // 3. Verifica colisão Vertical (Chão ou Teto)
    wall* hit_wall = check_collision_map(player, walls, num_walls);
		printf("hit_wall = %p\n", hit_wall);
    
    if (hit_wall != NULL) {
        
        // CASO A: Caindo (Bateu no Chão)
        if (player->vy > 0) {
            // Corrige posição: Fica exatamente em cima da parede
            player->y = (hit_wall->pos_y - hit_wall->height/2) - player->heigth/2;
            
            player->vy = 0;   // Para de cair
            
            // Se estava caindo (estado 2), aterra para estado 0 (em pé)
            if (player->idle == 2) {
                player->idle = 0; 
            }
        }
        
        // CASO B: Subindo (Bateu a Cabeça)
        else if (player->vy < 0) {
            // Corrige posição: Fica exatamente abaixo da parede
            player->y = (hit_wall->pos_y + hit_wall->height/2) + player->heigth/2;
            
            player->vy = 0; // Zera velocidade (começa a cair)
        }
    }
    // CASO C: Se não bateu em nada, verifica se está de pé ou no ar
    else {
        // Se estava de pé (idle != 2) e agora saiu do chão, vai para o ar
        if (player->idle == 0 || player->idle == 1) {
            player->idle = 2; // Estado: No ar
        }
    }
}



int update_enemy(square* enemy, square* player_1, ALLEGRO_FONT* font){																																//Função que atualiza o inimigo no jogo
		
		al_draw_text(font, al_map_rgb(255,255,255), 400, 75, 0, "Enemy HP:");
		char hp_str[4];
		sprintf(hp_str, "%d", enemy->hp);
		al_draw_text(font, al_map_rgb(255,255,255), 500, 75, 0, hp_str);

		if(collision_2D(enemy, player_1)){
			if (player_1->invincibility_timer == 0) {
				// Empurrão
				if (player_1->x < enemy->x) {
					player_1->x -= 20; 
					player_1->vy = -5; 
				} else {
					player_1->x += 20; 
					player_1->vy = -5;
				}
				
				player_1->hp--;
				player_1->invincibility_timer = 60; // Ativa cooldown
    		}

		}
		

		if(check_kill(enemy))
			enemy->hp--;

		int steps = (rand()%6);
		int trajetory = (rand()%2);
		int move = rand()%11;
		if(!move)
			square_move(enemy,steps, trajetory, X_BACKGROUND, Y_BACKGROUND);

		if(enemy->hp == 0)
			return 0;
		else
			return 1;
}

void update_vanishing_platforms(square *player, wall **walls, int num_walls) {
    for (int i = 0; i < num_walls; i++) {
        if (walls[i] != NULL && walls[i]->active == 1 && walls[i]->type == WALL_VANISHING) {
            printf("checando plataforma %d\n", i);
						printf("was_stepped = %d\n", walls[i]->was_stepped);
						printf("active = %d\n", walls[i]->active);
            // Verifica se o player está tocando nela AGORA
					
						//printf("checando plataforma %d: touching = %d\n", i, is_touching);
            if (check_collision_wall(player, walls[i])) {
                // Se tocou, marca que foi pisada
                walls[i]->was_stepped = 1;
            } 
            else {
                // Se NÃO está tocando, mas JÁ FOI pisada antes...
                if (walls[i]->was_stepped == 1) {
                    // ...Significa que o player saiu de cima!
                    walls[i]->active = 0; // DESAPARECE!
                    printf("Plataforma %d desapareceu!\n", i);
                }
            }
        }
    }
}

int main(){
	
	al_init();																																																//Faz a preparação de requisitos da biblioteca Allegro
	al_init_primitives_addon();																																												//Faz a inicialização dos addons das imagens básicas
	al_init_font_addon();																																													//Faz a inicialização dos addons de fontes	
	al_init_image_addon();

	al_install_keyboard();																																													//Habilita a entrada via teclado (eventos de teclado), no programa

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);																																						//Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();																																					//Cria a fila de eventos; todos os eventos (programação orientada a eventos) 
	ALLEGRO_FONT* font = al_create_builtin_font();																																							//Carrega uma fonte padrão para escrever na tela (é bitmap, mas também suporta adicionar fontes ttf)
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);																																			//Cria uma janela para o programa, define a largura (x) e a altura (y) da tela em píxeis (320x320, neste caso)

	al_register_event_source(queue, al_get_keyboard_event_source());																																		//Indica que eventos de teclado serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp));																																		//Indica que eventos de tela serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer));																																		//Indica que eventos de relógio serão inseridos na nossa fila de eventos
	
	
	
	printf("antes do menu\n");
	int option = 0;																																				//Chama a função que exibe o menu inicial do jogo
	printf("passei do menu\n");
	

	while(1){
	switch (option){
		case 0:
		{
			printf("entrando menu\n");
			option = menu(disp, X_SCREEN, Y_SCREEN);
			break;
		}


	 	case 1:
		{
			printf("iniciando jogo\n");
			square* player_1 = square_create(20, 20, 1, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																															//Cria o quadrado do primeiro jogador
			if (!player_1) return 1;																																												//Verificação de erro na criação do quadrado do primeiro jogador

			
			
			ALLEGRO_EVENT event;																																													//Variável que guarda um evento capturado, sua estrutura é definida em: https:		//www.allegro.cc/manual/5/ALLEGRO_EVENT
			al_start_timer(timer);																																													//Função que inicializa o relógio do programa
			unsigned char p1k = 0;																																											//Variáveis de controle de vida dos quadrados (jogadores)
			

			ALLEGRO_BITMAP* background = NULL;	
			background = map_create();	
			if (!background) {
				fprintf(stderr, "Failed to load background image!\n");
				return -1;
			}
			
			
			obstacle* map_obstacles[MAX_OBSTACLES]; 

			for(int i=0; i<MAX_OBSTACLES; i++) map_obstacles[i] = NULL;
			int obstacle_count = 0;

			map_obstacles[0] = obstacle_create(200, 900, 50, 50, "obstaculo1.png", 1, 15.0f);
			obstacle_count++;
			get_sprite_obstacle(map_obstacles[0], "obstaculo1.png");

			map_obstacles[1] = obstacle_create(700, 900, 50, 50, "obstaculo1.png", 1, 15.0f);
			obstacle_count++;
			get_sprite_obstacle(map_obstacles[1], "obstaculo1.png");

			map_obstacles[2] = obstacle_create(1035, 150 ,100 , 128, "obstaculo1.png", 1, 5.0f);
			obstacle_count++;
			get_sprite_obstacle(map_obstacles[2], "bomba.png");
		
			map_obstacles[3] = obstacle_create(1415, 200 ,120 , 156, "bomba.png", 1, 15.0f);
			obstacle_count++;
			get_sprite_obstacle(map_obstacles[3], "bomba.png");

			map_obstacles[4] = obstacle_create(2860, 225 ,120 , 156, "bomba.png", 1, 5.0f);
			obstacle_count++;
			get_sprite_obstacle(map_obstacles[4], "bomba.png");

			map_obstacles[5] = obstacle_create(3150, 1190 ,300 , 100, "varal.png", 1, 10.0f);
			obstacle_count++;
			get_sprite_obstacle(map_obstacles[5], "varal.png");

			wall* map_walls[MAX_WALLS];
			
			// Inicializa tudo como NULL para segurança
			for(int i=0; i<MAX_WALLS; i++) map_walls[i] = NULL;
			int wall_count = 0;

			// Cria as paredes e adiciona no array
			map_walls[0] = wall_create(1856, 65, 928, 957);  // Chão
			wall_count++;

			map_walls[1] = wall_create(200, 40, 650, 840);   // Obstáculo 1
			wall_count++;
			get_sprite_wall(map_walls[1], "plataforma3.png");

			map_walls[2] = wall_create(50, 50, 475, 725);  // Plataforma 2
			wall_count++;

			map_walls[3] = wall_create(50, 50, 375, 635);  // Plataforma 3
			wall_count++;

			map_walls[4] = wall_create(50, 50, 575, 635);  // Plataforma 4
			wall_count++;

			map_walls[5] = wall_create(50, 50, 500, 540);  // Plataforma 5
			wall_count++;

			map_walls[6] = wall_create(90, 25, 685, 562);
			wall_count++;
			get_sprite_wall(map_walls[6], "plataforma3.png");
			
			map_walls[7] = wall_create(50,350, 850,750);
			wall_count++;
			get_sprite_wall(map_walls[7], "plataforma2.png");
			
			map_walls[8] = wall_create(50,100, 850,450);
			wall_count++;
			get_sprite_wall(map_walls[8], "plataforma2.png");

			map_walls[9] = wall_create(50,50, 775,475);
			wall_count++;
			get_sprite_wall(map_walls[9], "plataforma1.png");
		
			map_walls[10] = wall_create(75,75, 912,342);
			wall_count++;
			get_sprite_wall(map_walls[10], "plataforma1.png");

			map_walls[11] = wall_create(50,50, 1125,362);
			wall_count++;
			get_sprite_wall(map_walls[11], "plataforma1.png");

			map_walls[12] = wall_create(200,40, 1300,362);
			wall_count++;
			get_sprite_wall(map_walls[12], "plataforma3.png");

			map_walls[13] = wall_create(50, 50, 1425,490);
			wall_count++;
			get_sprite_wall(map_walls[13], "plataforma1.png");

			map_walls[14] = wall_create(50, 50, 1500,370);
			wall_count++;
			get_sprite_wall(map_walls[14], "plataforma1.png");

			map_walls[15] = wall_create(130, 40, 1865, 310);
			wall_count++;
			get_sprite_wall(map_walls[15], "plataforma3.png");

			map_walls[16] = wall_create(38, 94, 1916, 222);
			wall_count++;
			get_sprite_wall(map_walls[16], "plataforma2.png");

			map_walls[17] = wall_create(50, 50, 2150,400);
			wall_count++;
			get_sprite_wall(map_walls[17], "plataforma1.png");

			map_walls[18] = wall_create(100, 100, 2500, 425);
			wall_count++;
			get_sprite_wall(map_walls[18], "plataforma1.png");

			map_walls[19] = wall_create(50, 50, 1210, 875);
			wall_count++;
			get_sprite_wall(map_walls[19], "plataforma1.png");

			map_walls[20] = wall_create(50, 50, 1475, 675);
			wall_count++;
			get_sprite_wall(map_walls[20], "plataforma1.png");

			map_walls[21] = wall_create(50, 50, 1750, 875);
			wall_count++;
			get_sprite_wall(map_walls[21], "plataforma1.png");
			
			map_walls[22] = wall_create(120, 40, 1335, 770);
			wall_count++;
			map_walls[22]->type = WALL_VANISHING; // Define como plataforma que desaparece
			get_sprite_wall(map_walls[22], "plataformaquebravel.png");

			map_walls[23] = wall_create(3000, 50, 3300, 1275);
			wall_count++;

			map_walls[24] = wall_create(100, 100, 3150, 450);
			wall_count++;
			get_sprite_wall(map_walls[24], "plataforma1.png");

			map_walls[25] = wall_create(32, 100, 3216, 350);
			wall_count++;
			get_sprite_wall(map_walls[25], "plataforma2.png");

			map_walls[26] = wall_create(50, 50, 3375, 300);
			wall_count++;
			get_sprite_wall(map_walls[26], "plataforma1.png");

			map_walls[27] = wall_create(50, 50, 3350, 475);
			wall_count++;
			get_sprite_wall(map_walls[27], "plataforma1.png");

			map_walls[28] = wall_create(50, 50, 3525, 400);
			wall_count++;
			get_sprite_wall(map_walls[28], "plataforma1.png");

			for(int i = 2; i<= 5; i++){
				if(map_walls[i]){
					get_sprite_wall(map_walls[i], "plataforma1.png");
				}
			}
			

			int map_width = al_get_bitmap_width(background); // Pega a largura real do mapa
			int map_height = al_get_bitmap_height(background); // Pega a altura real do mapa
			
			float camera_x = 0; // Inicializa a posição da câmera
			float camera_y = 0; // Inicializa a posição da câmera

			//al_start_timer(timer);																																													//Inicia o relógio do jogo
			//al_flush_event_queue(queue);

			printf("criei walls\n");

			typedef struct { float x, y, w, h; unsigned char triggered; } SpawnTrigger;
			SpawnTrigger enemy_spawn = { 0.0f, 930.0f, 50.0f, 100.0f, 0 }; // ajustar conforme posição da plataforma (x,y,w,h)
			square* enemy = NULL; // não criar ainda

		while(1){																																																//Laço servidor do jogo
			
			al_wait_for_event(queue, &event);																																									//Função que captura eventos da fila, inserindo os mesmos na variável de eventos
				if (p1k){																																													//Verifica se algum jogador foi morto 																																						//Limpe a tela atual para um fundo preto
					al_draw_bitmap(background, 0, 0, 0);	
					if (p1k) al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 40, Y_SCREEN/2-15, 0, "EMPATE!");																					//Se ambos foram mortos, declare um empate
					else if (p1k) al_draw_text(font, al_map_rgb(0, 0, 255), X_SCREEN/2 - 75, Y_SCREEN/2-15, 0, "JOGADOR 2 GANHOU!");																				//Se o primeiro jogador morreu, declare o segundo jogador vencedor
					al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 110, Y_SCREEN/2+5, 0, "PRESSIONE ESPAÇO PARA SAIR");																					//Indique o modo de conclusão do programa
					al_flip_display();																																												//Atualiza a tela

					if ((event.type == 10) && (event.keyboard.keycode == 75)) return 0;																																//Espera por um evento de teclado, de clique da tecla de espaço
					else if (event.type == 42) return 0; 																																								//Finaliza o jogo
				}
				else{																																																//Se nenhum quadrado morreu
					if (event.type == 30){																																											//O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)	
						
						camera_x = (player_1->x - X_SCREEN / 2);
						camera_y = (player_1->y - Y_SCREEN / 2) - 150 ;

						// Limite Esquerdo e Superior
						if (camera_x < 0) camera_x = 0;
						if (camera_y < 0) camera_y = 0;

						// Limite Direito e Inferior
						if (camera_x > map_width - X_SCREEN) camera_x = map_width - X_SCREEN;
						if (camera_y > map_height - Y_SCREEN) camera_y = map_height - Y_SCREEN;
										
						al_draw_bitmap_region(
											background, 
											camera_x,   // X de onde começar a cortar o mapa
											camera_y,   // Y de onde começar a cortar o mapa
											X_SCREEN,   // Largura da tela (o quanto cortar)
											Y_SCREEN,   // Altura da tela (o quanto cortar)
											0,          // X de destino na tela (sempre 0, desenha no canto superior esquerdo da tela)
											0,          // Y de destino na tela (sempre 0)
											0           // Flags
										);
															
						
						// COLISÃO COM OBSTÁCULOS
						for(int i = 0; i < obstacle_count; i++){
						// Verifica se colidiu especificamente com o obstáculo 'i'
							if (map_obstacles[i] != NULL && check_collision_obstacle(player_1, map_obstacles[i])) {
								
								// Verifica invencibilidade (para não morrer instantaneamente)
								if (player_1->invincibility_timer == 0) {
									printf("Colidiu com obstáculo %d\n", i);
									
									apply_knockback(player_1, map_obstacles[i]);
									player_1->hp--;
									
									// Ativa invencibilidade por ~2 segundos (60 frames)
									player_1->invincibility_timer = 60; 
								}
							}
						}

						for(int i = 0; i < obstacle_count; i++){
							draw_obstacle(map_obstacles[i], camera_x, camera_y);
						}


						for(int i = 1; i < wall_count; i++){
							wall_draw(map_walls[i], camera_x, camera_y);
						}


						
						if (!enemy && !enemy_spawn.triggered) {
							float px = player_1->x;
							float py = player_1->y;
							// AABB simples: verifica se jogador entrou na área de trigger
							if (px + player_1->width/2.0f > enemy_spawn.x - enemy_spawn.w/2.0f &&
								px - player_1->width/2.0f < enemy_spawn.x + enemy_spawn.w/2.0f &&
								py + player_1->heigth/2.0f > enemy_spawn.y - enemy_spawn.h/2.0f &&
								py - player_1->heigth/2.0f < enemy_spawn.y + enemy_spawn.h/2.0f) {
							// spawn do inimigo
								enemy = square_create(40, 40, 0, enemy_spawn.x+500, enemy_spawn.y - 30.0f, X_BACKGROUND, Y_BACKGROUND);
								if (enemy) {
									enemy->sprite = al_load_bitmap("enemy.png");
									if (!enemy->sprite) {
										fprintf(stderr, "Aviso: Não foi possível carregar enemy.png. Usando desenho padrão.\n");
									}
								}
								enemy_spawn.triggered = 1; // evita spawn repetido
							}
						}



						int debug_mode = 0;
						if(event.type == 10 && event.keyboard.keycode == ALLEGRO_KEY_H){
							debug_mode = !debug_mode;
						}

						if (debug_mode) {
						// Desenha Hitbox do Corpo em VERDE
						al_draw_rectangle(
							(player_1->x + player_1->body_box.offset_x) - player_1->body_box.width/2,
							(player_1->y + player_1->body_box.offset_y) - player_1->body_box.height/2,
							(player_1->x + player_1->body_box.offset_x) + player_1->body_box.width/2,
							(player_1->y + player_1->body_box.offset_y) + player_1->body_box.height/2,
							al_map_rgb(0, 255, 0), 1
						);
						}

						
						update_vanishing_platforms(player_1, map_walls, MAX_WALLS);
						update_life(player_1, font, background);
						update_position(player_1, map_walls, MAX_WALLS);            			
						update_physics(player_1, map_walls, MAX_WALLS);

					if (player_1->invincibility_timer > 0) {
						player_1->invincibility_timer--;
					}

					p1k = check_kill(player_1);																																						//Verifica se o primeiro jogador matou o segundo jogador

					if(enemy && update_enemy(enemy, player_1, font)){
						
						// Calcula posição em tela do inimigo
						float enemy_screen_x = enemy->x - camera_x;
						float enemy_screen_y = enemy->y - camera_y;
						
						// Desenha sprite PNG se existir, senão usa retângulo
						if (enemy->sprite) {
							al_draw_scaled_bitmap(enemy->sprite, 0, 0, 
												al_get_bitmap_width(enemy->sprite), 
												al_get_bitmap_height(enemy->sprite),
												enemy_screen_x - enemy->width/2, 
												enemy_screen_y - enemy->heigth/2, 
												enemy->width, 
												enemy->heigth, 0);
						} else {
							al_draw_rectangle(enemy_screen_x - enemy->width/2, enemy_screen_y - enemy->heigth/2, 
											  enemy_screen_x + enemy->width/2, enemy_screen_y + enemy->heigth/2, 
											  al_map_rgb(0, 255, 0), 3);
						}
					}
					
					float player_screen_x = player_1->x - camera_x;
					float player_screen_y = player_1->y - camera_y;


						al_draw_filled_rectangle(
							player_screen_x - player_1->width/2, 
							player_screen_y - player_1->heigth/2, 
							player_screen_x + player_1->width/2, 
							player_screen_y + player_1->heigth/2, 
							al_map_rgb(255, 0, 0)
						);
												
						al_flip_display();																																											//Insere as modificações realizadas nos buffers de tela
					}
					else if ((event.type == 10) || (event.type == 12)){																																				//Verifica se o evento é de botão do teclado abaixado ou levantado
						if (event.keyboard.keycode == 1) joystick_left(player_1->control);																															//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à esquerda)
						else if (event.keyboard.keycode == 4) joystick_right(player_1->control);																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)
						else if (event.keyboard.keycode == 23) joystick_up(player_1->control);																														//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para cima)
						else if (event.keyboard.keycode == 19) joystick_down(player_1->control);																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para baixo)
						if (event.keyboard.keycode == 217) joystick_ctr(player_1->control);
					}																																			
					else if (event.type == 42) break;																																								//Evento de clique no "X" de fechamento da tela. Encerra o programa graciosamente.
				}
			}
			
			al_destroy_bitmap(background);
			al_destroy_font(font);
			al_destroy_display(disp);
			al_destroy_timer(timer);
			al_destroy_event_queue(queue);
			square_destroy(player_1);
			if (enemy) {
				if (enemy->sprite) al_destroy_bitmap(enemy->sprite);
				free(enemy);
			}
		
			break;
		}
		case 2:
		{
			while(1){
				printf("iniciando controles\n");

				ALLEGRO_EVENT event;																																													//Variável que guarda um evento capturado, sua estrutura é definida em: https:		//www.allegro.cc/manual/5/ALLEGRO_EVENT
				al_wait_for_event(queue, &event);																																									//Função que captura

				ALLEGRO_BITMAP* control_image = NULL;
				control_image = al_load_bitmap("controles.png");
				al_draw_bitmap(control_image, 0, 0, 0);
				if (!control_image) {
					fprintf(stderr, "Failed to load control image!\n");
					return -1;
				}
				if(event.type == 10 && event.keyboard.keycode == ALLEGRO_KEY_SPACE){
					option = 0;
					printf("%d\n", option);
					break;
				}
				else if (event.type == 42) return 0; 																																								//Finaliza o jogo
				
				al_flip_display();
			}
			break;
		}
		case 3:
		{
			printf("saindo do jogo\n");
			return 0;

		}	
	return 0;
	}
	}
}		
