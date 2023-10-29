#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "Entity.h"

#ifndef CREATURE_H
#define CREATURE_H

enum {
	INTENTION_ATTACK = 0,
	INTENTION_SHIELD = 1,
	INTENTION_SPECIAL = 2
};

typedef struct CreatureEntity {
	Entity entity;
	int health;
	int shield;
	int intention;
	int intention_value;
} CreatureEntity;

void draw_creature_entity(CreatureEntity* ce, ALLEGRO_FONT* font, bool should_display_intention);

void roll_creature_intention(CreatureEntity* ce);

#endif