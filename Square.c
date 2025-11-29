#include <stdlib.h>
#include <stdio.h>
#include "Square.h"

#include "ChaoParede.h"

square* square_create(int heigth, int width, unsigned char face, float x, float y, float max_x, float max_y){			//Implementação da função "square_create"

	if ((x - width/2 < 0) || (x + width/2 > max_x) || (y - heigth/2 < 0) || (y + heigth/2 > max_y)) return NULL;												//Verifica se a posição inicial é válida; caso não seja, retorna NULL
	if (face > 3) return NULL;																															//Verifica se a face principal do quadrado é válida

	square *new_square = (square*) malloc(sizeof(square));																								//Aloca memória na heap para um novo quadrado
	if (!new_square) return NULL;																														//Se a alocação não deu certo, retorna erro													
	new_square->heigth = heigth;																															//Insere o tamanho do lado de um quadrado
	new_square->width = width;																															//Insere o tamanho do lado de um quadrado
	
    
    new_square->body_box.width = width / 2;    // Ex: 25 pixels de largura física
    new_square->body_box.height = heigth;      // Altura total
    new_square->body_box.offset_x = 0;         // Centralizado
    new_square->body_box.offset_y = 0;         // Centralizado
    new_square->body_box.active = 1;           // Sempre ativa

    // Configurando a Hitbox de ATAQUE (Ex: soco para frente)
    new_square->attack_box.width = 40;
    new_square->attack_box.height = 20;
    new_square->attack_box.offset_x = 30; // 30 pixels para frente do corpo
    new_square->attack_box.offset_y = 0;
    new_square->attack_box.active = 0;    // Começa desligada (só liga quando apertar botão)
    
    
	new_square->face = face;																															//Insere a indicação da face principal do quadrado
	new_square->hp = 5;																																	//Insere o total de pontos de vida de um quadrado (!)
	new_square->x = x;																																	//Insere a posição inicial central de X
	new_square->y = y;																																	//Insere a posição inicial central de Y
	new_square->idle = 0;
	new_square->vy = 0;
	new_square->sprite = NULL;  // Sprite inicialmente vazio
	new_square->control = joystick_create();																											//Insere o elemento de controle do quadrado
	new_square->invincibility_timer = 0; // Inicializa o contador de invencibilidade
    new_square->original_height = heigth;  // NOVO: guarda altura original
    return new_square;																																	//Retorna o novo quadrado
}

void square_move(square *element, char steps, unsigned char trajectory, float max_x, float max_y){									//Implementação da função "square_move"

	if (!trajectory){ if ((element->x - steps*SQUARE_STEP) - element->width/2 >= 0) element->x = element->x - steps*SQUARE_STEP;} 						//Verifica se a movimentação para a esquerda é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 1){ if ((element->x + steps*SQUARE_STEP) + element->width/2 <= max_x) element->x = element->x + steps*SQUARE_STEP;}			//Verifica se a movimentação para a direita é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 2){ if ((element->y - steps*SQUARE_STEP) - element->heigth/2 >= 0) element->y = element->y - steps*SQUARE_STEP;}				//Verifica se a movimentação para cima é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 3){ if ((element->y + steps*SQUARE_STEP) + element->heigth/2 <= max_y) element->y = element->y + steps*SQUARE_STEP;}			//Verifica se a movimentação para baixo é desejada e possível; se sim, efetiva a mesma
else if (trajectory == 4 ){
    // Só permite alterar agachamento se NÃO estiver no ar (idle != 2)
    if (element->idle != 2) {

        // Se está em pé (0) e apertou o botão, agacha
        if(element->control->ctr == 1 && element->idle == 0 && element->control->ctr_pressed == 0){
            printf("agachando\n");
            element->heigth = element->original_height / 2;  // Usa original_height!
            element->body_box.height = element->heigth;
            element->idle = 1;
            element->control->ctr = 0; // Reseta o estado do botão
            element->control->ctr_pressed = 1; // Marca que o botão foi processado
        } 
        // Se já está agachado (1) e NÃO está apertando CTR, levanta
        else if(element->control->ctr == 1 && element->control->ctr_pressed == 1){
            printf("levantando\n");
            element->heigth = element->original_height;  // Restaura para original
            element->body_box.height = element->heigth;
            element->idle = 0;
            element->control->ctr = 0; // Reseta o estado do botão
            element->control->ctr_pressed = 0; // Reseta o processamento do botão
        }  

    }
}

}


void square_destroy(square *element){																													//Implementação da função "square_destroy"

	joystick_destroy(element->control);																													//Destrói o controle do quadrado
	free(element);																																		//Libera a memória do quadrado na heap
}


