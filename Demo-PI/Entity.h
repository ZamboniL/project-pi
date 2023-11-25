#include <allegro5/allegro.h>

#ifndef Entity_H
#define Entity_H

typedef struct Entity {
	int x;
	int y;
	int width;
	int height;
	int sheet_column;
	int sheet_row;
	ALLEGRO_BITMAP* bitmap;
	bool is_mouse_over;
} Entity;

bool mark_if_mouse_is_over_entity(ALLEGRO_MOUSE_EVENT mouse, Entity* entity);

#endif