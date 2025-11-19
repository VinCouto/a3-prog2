#include <stdlib.h>
#include <stdio.h>
#include "Square.h"


square* square_create(int heigth, int width, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){			//Implementação da função "square_create"

	if ((x - width/2 < 0) || (x + width/2 > max_x) || (y - heigth/2 < 0) || (y + heigth/2 > max_y)) return NULL;												//Verifica se a posição inicial é válida; caso não seja, retorna NULL
	if (face > 3) return NULL;																															//Verifica se a face principal do quadrado é válida

	square *new_square = (square*) malloc(sizeof(square));																								//Aloca memória na heap para um novo quadrado
	if (!new_square) return NULL;																														//Se a alocação não deu certo, retorna erro													
	new_square->heigth = heigth;																															//Insere o tamanho do lado de um quadrado
	new_square->width = width;																															//Insere o tamanho do lado de um quadrado
	new_square->face = face;																															//Insere a indicação da face principal do quadrado
	new_square->hp = 5;																																	//Insere o total de pontos de vida de um quadrado (!)
	new_square->x = x;																																	//Insere a posição inicial central de X
	new_square->y = y;																																	//Insere a posição inicial central de Y
	new_square->idle = 0;
	new_square->vy = 0;	
	new_square->control = joystick_create();																											//Insere o elemento de controle do quadrado
	return new_square;																																	//Retorna o novo quadrado
}

void square_move(square *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y){									//Implementação da função "square_move"

	if (!trajectory){ if ((element->x - steps*SQUARE_STEP) - element->width/2 >= 0) element->x = element->x - steps*SQUARE_STEP;} 						//Verifica se a movimentação para a esquerda é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 1){ if ((element->x + steps*SQUARE_STEP) + element->width/2 <= max_x) element->x = element->x + steps*SQUARE_STEP;}			//Verifica se a movimentação para a direita é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 2){ if ((element->y - steps*SQUARE_STEP) - element->heigth/2 >= 0) element->y = element->y - steps*SQUARE_STEP;}				//Verifica se a movimentação para cima é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 3){ if ((element->y + steps*SQUARE_STEP) + element->heigth/2 <= max_y) element->y = element->y + steps*SQUARE_STEP;}			//Verifica se a movimentação para baixo é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 4 ){
        
        // Só permite alterar agachamento se NÃO estiver no ar (idle != 2)
        if (element->idle != 2) {
            
            // Se já está agachado (1), levanta
            if(element->idle == 1) {
                printf("levantando\n");
                element->heigth = element->heigth * 2; // Restaura altura
                element->idle = 0;                     // Estado: Em pé
                element->control->ctr = 0;
            } 
            // Se está em pé (0) e apertou o botão, agacha
            else if(element->control->ctr == 1 && element->idle == 0){
                printf("agachando\n");
                element->heigth = element->heigth / 2; // Diminui altura
                element->idle = 1;                     // Estado: Agachado
                element->control->ctr = 0;
            }  
        }
    }

}




void square_apply_physics(square *element, unsigned short floor_y) {
    // 1. APLICA A GRAVIDADE
    // Aumenta a velocidade para baixo a cada frame
    element->vy += 1.0f; 

    // 2. ATUALIZA A POSIÇÃO Y
    // Soma a velocidade atual à posição
    element->y += (int)element->vy;

    // 3. DETECTA O CHÃO (COLISÃO)
    // Verifica se o pé do quadrado (y + metade da altura) passou do chão
    if (element->y + element->heigth/2 >= floor_y) {
        
        // Coloca ele exatamente em cima do chão
        element->y = floor_y - element->heigth/2;
        
        // Zera a velocidade (parou de cair)
        element->vy = 0;
        
        // LÓGICA DOS ESTADOS (IDLE)
        // Se ele estava no ar (estado 2), ele aterra e fica em pé (estado 0)
        if (element->idle == 2) {
            element->idle = 0;
        }
        // Se ele estava agachado (1), continua agachado.
        // Se estava em pé (0), continua em pé.
    } 
    else {
        // 4. SE NÃO ESTÁ NO CHÃO, ESTÁ NO AR
        // Se ele não está agachado (1), defina como pulando/caindo (2)
        // Isso impede que ele agache no ar
        if (element->idle != 1) {
            element->idle = 2;
        }
    }
}




void square_destroy(square *element){																													//Implementação da função "square_destroy"

	joystick_destroy(element->control);																													//Destrói o controle do quadrado
	free(element);																																		//Libera a memória do quadrado na heap
}


