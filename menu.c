#include <allegro5/allegro5.h>																																											//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																										//Biblioteca
#include <allegro5/allegro_primitives.h>																																								//Biblioteca de figuras básicas
#include <allegro5/allegro_image.h>
#include <stdio.h> 
#include <stdlib.h>

#include "Square.h"



int menu(unsigned int X_SCREEN, unsigned int Y_SCREEN){
  
  int state_menu = 0;
  
  ALLEGRO_BITMAP *menu_image = NULL;
  menu_image = al_load_bitmap("menu.png");
  if(!menu_image){
    fprintf(stderr, "Failed to load menu image!\n");
    return 0;
  }
  
  ALLEGRO_DISPLAY* disp = NULL;
  if(!disp){
    fprintf(stderr, "Failed to create display!\n");
    al_destroy_bitmap(menu_image);
    return 0;
  }
  
  
  ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
  if(!queue){
    fprintf(stderr, "Failed to create event queue!\n");
    al_destroy_bitmap(menu_image);
    return 0;
  }
  
  
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  
  
  ALLEGRO_EVENT event;
  
  while(state_menu != 2){
    
    al_wait_for_event(queue, &event);

    al_draw_bitmap(menu_image, 0, 0, 0);
    
    al_flip_display();
    
    /*
    if(event.type == 10 && event.keyboard.keycode == 75){
      state_menu = 1;
    }
  
    
    if(state_menu == 1){
      al_draw_text(al_create_builtin_font(), al_map_rgb(255, 0, 0), X_SCREEN / 2, Y_SCREEN - 100, ALLEGRO_ALIGN_CENTRE, "PRESS ENTER TO START");
    }


    if(event.type == 10 && event.keyboard.keycode == ALLEGRO_KEY_ENTER){
      return state_menu;
    }*/

    if(event.type == 21){ // ALLEGRO_EVENT_KEY_DOWN
      // 59 é o código para ALLEGRO_KEY_ENTER
      if (event.keyboard.keycode == 59) state_menu = 1; 
      // 1 é o código para ALLEGRO_KEY_ESCAPE
      else if (event.keyboard.keycode == 1) state_menu = 2; 
    }
    else if (event.type == 42) { // ALLEGRO_EVENT_DISPLAY_CLOSE
        state_menu = 2;
    }
  }

  al_destroy_bitmap(menu_image);
  al_destroy_event_queue(queue);


  return state_menu;
}
  

