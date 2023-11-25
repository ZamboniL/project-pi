#include <allegro5/allegro.h>

#ifndef MOUSE_H
#define MOUSE_H

enum {
  MOUSE_BUTTON_LEFT = 1,
  MOUSE_BUTTON_RIGHT = 2
};

extern ALLEGRO_MOUSE_EVENT mouse;

extern int mouse_key[3];
extern bool mouse_position_updated;

void mouse_init();
void update_mouse(ALLEGRO_EVENT event);

#endif