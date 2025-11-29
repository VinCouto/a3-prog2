#ifndef __SQUARE__
#define __SQUARE__

#include "Joystick.h"
#include <allegro5/allegro5.h>

// Forward declaration of wall struct to avoid circular dependency
typedef struct wall wall;

#define SQUARE_STEP 10
#define GRAVIDADE 0.8f
#define FORCA_PULO -15.0f
#define CHAO_Y 500

typedef struct {
    short offset_x;
    short offset_y;
    unsigned short width;
    unsigned short height;
    unsigned char active;
} Hitbox;

typedef struct {
    int heigth;
    int width;
    unsigned char face;
    unsigned char hp;
    float x; // posição X (float para suavizar física)
    float y; // posição Y (float para suavizar física)
    unsigned short idle;
    float vy;
    Hitbox body_box;
    Hitbox attack_box;
    joystick *control;
    ALLEGRO_BITMAP *sprite;  // Sprite do personagem (pode ser NULL)
    int invincibility_timer; // Contador de frames invencível
    float original_height;  // NOVO: guarda altura original

} square;

square* square_create(int heigth, int width, unsigned char face, float x, float y, float max_x, float max_y);
void square_move(square *element, char steps, unsigned char trajectory, float max_x, float max_y);
void square_destroy(square *element);

#endif
#ifndef __SQUARE__ 																																	//Guardas de inclusão
#define __SQUARE__																																	//Guardas de inclusão																															//Quantidade de pixels que um quadrado se move por passo

#include "Joystick.h"	


// Forward declaration of wall struct to avoid circular dependency
typedef struct wall wall;

#define SQUARE_STEP 10																																//Tamanho, em pixels, de um passo do quadrado
#define GRAVIDADE 0.7f     // Quanto a velocidade aumenta para baixo a cada frame
#define FORCA_PULO -15.0f  // Velocidade instantânea para cima (negativa pois Y cresce para baixo)
#define CHAO_Y 500         // A altura Y onde fica o chão (por enquanto, fixo)


typedef struct {
    short offset_x; // Deslocamento em relação ao centro do X do player
    short offset_y; // Deslocamento em relação ao centro do Y do player
    unsigned short width;  // Largura da caixa de colisão
    unsigned short height; // Altura da caixa de colisão
    unsigned char active;  // 1 = Ativa, 0 = Inativa (útil para ataques)
} Hitbox;

typedef struct {																																	//Definição da estrutura de um quadrado
	int heigth;																																//Tamanmho da lateral de um quadrado
	int width;
	unsigned char face;																																//A face principal do quadrado, algo como a sua "frente"
	unsigned char hp;																																//Quantidade de vida do quadrado, em unidades (!)
	unsigned short x;																																//Posição X do centro do quadrado
	unsigned short y;																															//Posição Y do centro do quadrado
	unsigned short idle;
	float vy;
	Hitbox body_box;   // Caixa do corpo (Hurtbox - onde ele apanha)
    Hitbox attack_box; // Caixa de ataque (Hitbox - onde ele bate)
	joystick *control;																																//Elemento de controle do quadrado no jogo
} square;																																			//Definição do nome da estrutura

square* square_create(int heigth, int width, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);		//Protótipo da função de criação de um quadrado
void square_move(square *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);								//Protótipo da função de movimentação de um quadrado
void square_destroy(square *element);																												//Protótipo da função de destruição de um quadrado

#endif																																				//Guardas de inclusão