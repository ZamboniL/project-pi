#include "Player.h"
#include "CreatureEntity.h"
#include "CombatArena.h"
#include "Card.h"

void play_card() {
	Card* card = &player.hand[player.card_being_dragged];
	CreatureEntity* target;

	if (player.hovering_arena_entity == HOVERING_HERO) {
		target = &arena.hero;
	}
	else {
		target = &arena.enemies[player.hovering_arena_entity];
	}

	target->shield += card->shield;
	player.mana -= card->cost;

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

	discard_card(player.card_being_dragged);
}

void apply_shield(CreatureEntity* target, int value) {
}