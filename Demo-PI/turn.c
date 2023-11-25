#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "screen.h"
#include "Turn.h"
#include "Mouse.h"
#include "Entity.h"

#ifndef strcat_s
#define strcat_s(dest, size, src) strncat((dest), (src), (size))
#endif

Turn turn;
int new_turn_timer = 0;

void turn_init(ALLEGRO_BITMAP* bitmap) {
  turn.value = 1;
  turn.is_new_turn = true;
  turn.entity.x = 460;
  turn.entity.y = 340;
  turn.entity.width = 160;
  turn.entity.height = 40;
  turn.can_change_turn = true;
  turn.entity.bitmap = bitmap;
}

void update_turn() {
  mark_if_mouse_is_over_entity(mouse, &turn.entity);

  if (mouse_key[MOUSE_BUTTON_LEFT] && turn.entity.is_mouse_over && turn.can_change_turn) {
    turn.value++;
    turn.is_new_turn = true;
    turn.can_change_turn = false;
    new_turn_timer = 180;
  }
  else {
    turn.is_new_turn = false;
  }

  if (!new_turn_timer) {
    turn.can_change_turn = true;
  }
  else if (new_turn_timer > 0) {
    new_turn_timer--;
  }
}


void draw_turn(ALLEGRO_FONT* font) {
  if (turn.can_change_turn) {
    if (turn.entity.is_mouse_over) {
      al_draw_bitmap_region(
        turn.entity.bitmap,
        0,
        turn.entity.height,
        turn.entity.width,
        turn.entity.height,
        turn.entity.x,
        turn.entity.y,
        0
      );
    }
    else {
      al_draw_bitmap_region(turn.entity.bitmap, 0, 0, turn.entity.width, turn.entity.height, turn.entity.x, turn.entity.y, 0);
    }
  }
  else {
    al_draw_bitmap_region(
      turn.entity.bitmap,
      0,
      turn.entity.height * 2,
      turn.entity.width,
      turn.entity.height,
      turn.entity.x,
      turn.entity.y,
      0
    );
  }

}