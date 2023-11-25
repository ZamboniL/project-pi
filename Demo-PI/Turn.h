#include <allegro5/allegro_font.h>
#include "Entity.h"

#ifndef TURN_H
#define TURN_H

typedef struct Turn {
  Entity entity;
  int value;
  bool is_new_turn;
  bool can_change_turn;
} Turn;

extern Turn turn;

void turn_init(ALLEGRO_BITMAP* turn_bitmap);
void update_turn();
void draw_turn(ALLEGRO_FONT* font);

#endif