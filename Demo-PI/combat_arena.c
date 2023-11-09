#include "CombatArena.h"
#include "Player.h"
#include "Entity.h"
#include "CreatureEntity.h"
#include "allegro5/allegro.h"

CombatArena arena;

void mark_if_mouse_is_over_arena_entity(ALLEGRO_MOUSE_EVENT mouse) {
	player.is_hovering_arena_entity = false;

	mark_if_mouse_is_over_entity(mouse, &arena.hero.entity);
	if (arena.hero.entity.is_mouse_over) {
		player.hovering_arena_entity = HOVERING_HERO;
		player.is_hovering_arena_entity = true;
		return;
	}

	for (int i = 0; i < arena.enemies_quantity; i++) {
		if (arena.enemies[i].health == 0) {
			continue;
		}

		mark_if_mouse_is_over_entity(mouse, &arena.enemies[i].entity);
		if (arena.enemies[i].entity.is_mouse_over) {
			player.hovering_arena_entity = i;
			player.is_hovering_arena_entity = true;
			return;
		}
	}
}

void roll_enemy_intentions() {

	for (int i = 0; i < arena.enemies_quantity; i++) {
		if (arena.enemies[i].health == 0) {
			continue;
		}

		roll_creature_intention(&arena.enemies[i]);
	}
}

void arena_init() {
	CreatureEntity enemy = { .entity.x = 450, .entity.y = 100, .entity.width = 100, .entity.height = 150, .health = 10, .shield = 10 };
	CreatureEntity hero = { .entity.x = 100, .entity.y = 100, .entity.width = 100, .entity.height = 150, .health = 50 };

	arena.hero = hero;
	arena.enemies_quantity = 1;
	arena.enemies[0] = enemy;

	roll_enemy_intentions();
}


void draw_arena_entities(ALLEGRO_FONT* font) {
	draw_creature_entity(&arena.hero, font, false);
	
	for (int i = 0; i < arena.enemies_quantity; i++) {
		if (arena.enemies[i].health == 0) {
			continue;
		}

		draw_creature_entity(&arena.enemies[i], font, true);
	}
}