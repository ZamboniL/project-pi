#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "game_state.h"
#include "Mouse.h"
#include "CombatArena.h"
#include "Player.h"
#include "Entity.h"
#include "CreatureEntity.h"
#include "Turn.h"

CombatArena arena;

void mark_if_mouse_is_over_arena_entity(ALLEGRO_MOUSE_EVENT mouse) {
	player.is_hovering_arena_entity = false;

	mark_if_mouse_is_over_entity(mouse, &arena.hero.entity);
	if (arena.hero.entity.is_mouse_over) {
		player.hovering_arena_entity = arena.hero.id;
		player.is_hovering_arena_entity = true;
		return;
	}

	for (int i = 0; i < arena.enemies_quantity; i++) {
		if (arena.enemies[i].health == 0) {
			continue;
		}

		mark_if_mouse_is_over_entity(mouse, &arena.enemies[i].entity);
		if (arena.enemies[i].entity.is_mouse_over) {
			player.hovering_arena_entity = arena.enemies[i].id;
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

void arena_init(ALLEGRO_BITMAP* hero_bitmap, ALLEGRO_BITMAP* enemy_bitmap) {
	CreatureEntity enemy = create_creature_by_id(CREATURE_ID_CANCER);

	CreatureEntity hero = create_creature_by_id(CREATURE_ID_HERO);
	hero.entity.bitmap = hero_bitmap;
	enemy.entity.bitmap = enemy_bitmap;

	arena.hero = hero;
	arena.enemies_quantity = 1;
	arena.enemies[0] = enemy;

	roll_enemy_intentions();
}

void update_arena_entities_animation() {
	if (arena.hero.animation.type != IDLE && arena.hero.animation.cancellable) {
		change_creature_animation(&arena.hero, IDLE);
	}

	update_animation(&arena.hero.animation);

	for (int i = 0; i < arena.enemies_quantity; i++) {
		if (arena.enemies[i].health == 0) {
			continue;
		}

		if (arena.enemies[i].animation.type != IDLE && arena.enemies[i].animation.cancellable) {
			change_creature_animation(&arena.enemies[i], IDLE);
		}

		update_animation(&arena.enemies[i].animation);
	}
}

void update_creatures_status() {
	if (arena.hero.headache > 0) {
		arena.hero.headache--;
		arena.hero.health -= 4;
	}

	if (arena.hero.slowness > 0) {
		arena.hero.slowness--;
	}

	if (arena.hero.weakness > 0) {
		arena.hero.weakness--;
	}

	if (arena.hero.quickness > 0) {
		arena.hero.quickness--;
	}

	for (int i = 0; i < arena.enemies_quantity; i++) {
		if (arena.enemies[i].headache > 0) {
			arena.enemies[i].headache--;
			arena.enemies[i].health -= 4;
		}

		if (arena.enemies[i].slowness > 0) {
			arena.enemies[i].slowness--;
		}

		if (arena.enemies[i].weakness > 0) {
			arena.enemies[i].weakness--;
		}

		if (arena.enemies[i].quickness > 0) {
			arena.enemies[i].quickness--;
		}
	}
}

void update_arena(GameState* game_state) {
	if (arena.hero.health <= 0 && game_state->screen != SCREEN_DEFEAT) {
		change_game_state_screen(game_state, SCREEN_DEFEAT);
		return;
	}

	if (arena.enemies[0].health <= 0 && game_state->screen != SCREEN_WIN) {
		change_game_state_screen(game_state, SCREEN_WIN);
		return;
	}

	update_arena_entities_animation();

	if (turn.is_new_turn) {
		update_creatures_status();
		roll_enemy_intentions();
	}

	mark_if_mouse_is_over_arena_entity(mouse);
}

void draw_arena_entities(ALLEGRO_FONT* font) {
	draw_creature_entity(&arena.hero, font);

	for (int i = 0; i < arena.enemies_quantity; i++) {
		if (arena.enemies[i].health == 0) {
			continue;
		}

		draw_creature_entity(&arena.enemies[i], font);
	}
}

CreatureEntity* get_creature_by_id(int id) {
	if (arena.hero.id == id) {
		return &arena.hero;
	}
	else {
		for (int i = 0; i < arena.enemies_quantity; i++) {
			if (arena.enemies[i].id == id) {
				return &arena.enemies[i];
			}
		}
	}
}