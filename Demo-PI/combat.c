#include "Table.h"
#include "CombatArena.h"
#include "Card.h"

void play_card(Table* table, CombatArena* arena) {
	Card* card = &table->hand[table->card_being_dragged];
	CreatureEntity* target;

	if (table->hovering_arena_entity == HOVERING_HERO) {
		target = &arena->hero;
	}
	else {
		target = &arena->enemies[table->hovering_arena_entity];
	}

	target->shield += card->shield;

	if (!target->shield) {
		target->health -= card->damage;
	}
	else if (card->damage >= target->shield) {
		int difference = target->shield - card->damage;
		target->health += difference;
		target->shield = 0;
	}
	else if(card->damage) {
		target->shield -= card->damage;
	}

	discard_card(table, table->card_being_dragged);
}

void apply_shield(CreatureEntity* target, int value) {
}