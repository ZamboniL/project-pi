#include "Entity.h"

void mark_if_mouse_is_over_entity(ALLEGRO_MOUSE_EVENT mouse, Entity* entity) {
	entity->is_mouse_over = mouse.x >= entity->x && mouse.x <= entity->x + entity->width &&
		mouse.y >= entity->y && mouse.y <= entity->y + entity->height;
}