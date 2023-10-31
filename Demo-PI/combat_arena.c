#include "CombatArena.h"
#include "Table.h"
#include "Entity.h"
#include "CreatureEntity.h"
#include "allegro5/allegro.h"


void mark_if_mouse_is_over_arena_entity(ALLEGRO_MOUSE_EVENT mouse, CombatArena* arena, Table* table) {
	table->is_hovering_arena_entity = false;

	mark_if_mouse_is_over_entity(mouse, &arena->hero);
	if (arena->hero.entity.is_mouse_over) {
		table->hovering_arena_entity = HOVERING_HERO;
		table->is_hovering_arena_entity = true;
		return;
	}

	for (int i = 0; i < arena->enemies_quantity; i++) {
		if (arena->enemies[i].health == 0) {
			continue;
		}

		mark_if_mouse_is_over_entity(mouse, &arena->enemies[i]);
		if (arena->enemies[i].entity.is_mouse_over) {
			table->hovering_arena_entity = i;
			table->is_hovering_arena_entity = true;
			return;
		}
	}
}

void roll_enemy_intentions(CombatArena* arena) {

	for (int i = 0; i < arena->enemies_quantity; i++) {
		if (arena->enemies[i].health == 0) {
			continue;
		}

		roll_creature_intention(&arena->enemies[i]);
	}
}

CombatArena create_arena() {
	CreatureEntity enemy = { .entity.x = 450, .entity.y = 100, .entity.width = 100, .entity.height = 150, .health = 10, .shield = 25 };
	CreatureEntity hero = { .entity.x = 100, .entity.y = 100, .entity.width = 100, .entity.height = 150, .health = 50 };
	CombatArena arena = { .hero = hero, .enemies = {enemy}, .enemies_quantity = 1 };

	roll_enemy_intentions(&arena);

	return arena;
}


void draw_arena_entities(CombatArena* arena, ALLEGRO_FONT* font) {
	draw_creature_entity(&arena->hero, font, false);
	
	for (int i = 0; i < arena->enemies_quantity; i++) {
		if (arena->enemies[i].health == 0) {
			continue;
		}

		draw_creature_entity(&arena->enemies[i], font, true);
	}
}