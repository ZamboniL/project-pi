#include <allegro5/allegro5.h>
#include "screen.h"
#include "Mouse.h"

ALLEGRO_MOUSE_EVENT mouse;

bool mouse_position_updated = false;
int mouse_key[3];

void mouse_init() {
  memset(&mouse, 0, sizeof(ALLEGRO_MOUSE_EVENT));
}

void update_mouse(ALLEGRO_EVENT event) {
  switch (event.type) {
  case ALLEGRO_EVENT_TIMER:
    mouse_key[MOUSE_BUTTON_LEFT] = 0;
    mouse_key[MOUSE_BUTTON_RIGHT] = 0;
    mouse_position_updated = false;
    break;
  case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
    mouse = event.mouse;
    mouse_key[event.mouse.button] = 1;
    break;
  case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
    mouse = event.mouse;
    mouse_key[event.mouse.button] = 2;
    break;
  case ALLEGRO_EVENT_MOUSE_AXES:
    mouse = event.mouse;
    mouse_position_updated = true;
    break;
  }
}
