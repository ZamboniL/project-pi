#include "CreatureEntity.h"
#include "game_state.h"

#ifndef COMBAT_ARENA_H
#define COMBAT_ARENA_H

#define ARENA_MAX_CAPACITY 4

typedef struct CombatArena {
	CreatureEntity hero;
	CreatureEntity enemies[ARENA_MAX_CAPACITY];
	int enemies_quantity;
} CombatArena;

extern CombatArena arena;

void mark_if_mouse_is_over_arena_entity(ALLEGRO_MOUSE_EVENT mouse);

void roll_enemy_intentions();

void arena_init(ALLEGRO_BITMAP* hero, ALLEGRO_BITMAP* enemy);

void update_arena(GameState* game_state);

void draw_arena_entities(ALLEGRO_FONT* font);

CreatureEntity* get_creature_by_id(int id);

#endif