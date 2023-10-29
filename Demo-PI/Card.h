#include <allegro5/allegro_font.h>
#include "Entity.h"

#ifndef CARD_H
#define CARD_H

enum {
	CARD_NULL = 0,
	CARD_VACCINE = 1,
	CARD_ANTIBACTERIAL = 2
};

typedef struct Card {
	int id;
	Entity entity;
	const char *title;
	int cost;
	int damage;
	int shield;
} Card;

Card create_card(int id, const char* title, int cost, int damage, int shield);

Card create_card_from_id(int id);

void draw_card(Card* card, ALLEGRO_FONT* font);

#endif