#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "Animation.h"
#include "Entity.h"
#include "Card.h"

#ifndef CREATURE_H
#define CREATURE_H

#define MAX_POSSIBLE_PLAYS 5

enum Intention {
	INTENTION_ATTACK = 0,
	INTENTION_SHIELD = 1,
	INTENTION_SPECIAL = 2
};

enum CreatureId {
	CREATURE_ID_HERO = 0,
	CREATURE_ID_CANCER = 1,
};

typedef struct Play {
	int target;
	int chance;
	Card card;
} Play;

// a structure to store animation coordinates for a creature's each action on a sprite sheet

typedef struct CreatureEntity {
	Entity entity;
	int animation_coordinates[4][2];
	Animation animation;
	int id;
	int health;
	int max_health;
	int weakness;
	int strength;
	int quickness;
	int slowness;
	int headache;
	int shield;
	int play[2];
	int possible_plays_quantity;
	Play possible_plays[MAX_POSSIBLE_PLAYS];
	int max_time_for_next_action;
	int last_action_time;
} CreatureEntity;

void icons_init(ALLEGRO_BITMAP* shield);

void draw_creature_entity(CreatureEntity* ce, ALLEGRO_FONT* font);

void roll_creature_intention(CreatureEntity* ce);

void change_creature_animation(CreatureEntity* ce, enum AnimationType type);

CreatureEntity create_creature_by_id(enum CreatureId id);

#endif