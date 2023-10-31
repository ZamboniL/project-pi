#include "CreatureEntity.h"
#include "Table.h"

#ifndef COMBAT_ARENA_H
#define COMBAT_ARENA_H

#define ARENA_MAX_CAPACITY 4

typedef struct CombatArena {
	CreatureEntity hero;
	CreatureEntity enemies[ARENA_MAX_CAPACITY];
	int enemies_quantity;
} CombatArena;

void mark_if_mouse_is_over_arena_entity(ALLEGRO_MOUSE_EVENT mouse, CombatArena* arena, Table* table);

void roll_enemy_intentions(CombatArena* arena);

CombatArena create_arena();

void draw_arena_entities(CombatArena* arena, ALLEGRO_FONT* font);

#endif