#ifndef __JOYSTICK__ 																												//Guardas de inclusão 
#define __JOYSTICK__																												//Guardas de inclusão 

typedef struct {																													//Definição da estrutura de um controle 
	unsigned char right;																											//Botão de movimentação à direta 
	unsigned char left;																												//Botão de movimentação à esquerda 
	unsigned char up;																												//Botão de movimentação para cima 
	unsigned char down;																												//Botão de agachar																											//Botão de movimentação para baixo 
	unsigned char ctr;
} joystick;																															//Definição do nome da estrutura 

joystick* joystick_create();																										//Protótipo da função de criação de um controle 
void joystick_destroy(joystick *element);																							//Protótipo da função de destruição de um controle 
void joystick_right(joystick *element);																								//Protótipo da função de ativação/desativação do botão de movimentação à direita 
void joystick_right_press(joystick *element);
void joystick_right_release(joystick *element);
void joystick_left(joystick *element);																								//Protótipo da função de ativação/desativação do botão de movimentação à esquerda 
void joystick_left_press(joystick *element);
void joystick_left_release(joystick *element);
void joystick_up(joystick *element);																								//Protótipo da função de ativação/desativação do botão de movimentação para cima 
void joystick_up_press(joystick *element);
void joystick_up_release(joystick *element);
void joystick_down(joystick *element);																								//Protótipo da função de ativação/desativação do botão de movimentação para baixo 
void joystick_down_press(joystick *element);
void joystick_down_release(joystick *element);
void joystick_ctr(joystick *element);

#endif																																//Guardas de inclusão 