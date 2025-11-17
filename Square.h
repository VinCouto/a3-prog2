#ifndef __SQUARE__ 																																	//Guardas de inclusão
#define __SQUARE__																																	//Guardas de inclusão																															//Quantidade de pixels que um quadrado se move por passo

#include "Joystick.h"																																//Estrutura e procedimentos relacionados ao controle do quadrado

#define SQUARE_STEP 10																																//Tamanho, em pixels, de um passo do quadrado

typedef struct {																																	//Definição da estrutura de um quadrado
	int heigth;																																//Tamanmho da lateral de um quadrado
	int width;
	unsigned char face;																																//A face principal do quadrado, algo como a sua "frente"
	unsigned char hp;																																//Quantidade de vida do quadrado, em unidades (!)
	unsigned short x;																																//Posição X do centro do quadrado
	unsigned short y;																															//Posição Y do centro do quadrado
	unsigned short idle;
	joystick *control;																																//Elemento de controle do quadrado no jogo
} square;																																			//Definição do nome da estrutura

square* square_create(int heigth, int width, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);		//Protótipo da função de criação de um quadrado
void square_move(square *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);								//Protótipo da função de movimentação de um quadrado
void square_destroy(square *element);																												//Protótipo da função de destruição de um quadrado

#endif																																				//Guardas de inclusão