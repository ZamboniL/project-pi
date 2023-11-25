#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include "Entity.h"
#include "Animation.h"

#ifndef CARD_H
#define CARD_H

enum CardId {
	CARD_NULL = 0,
	CARD_VACCINE = 1,
	CARD_VERMIFUGE = 2,
	CARD_REST = 3,
	CARD_CAFFEINE = 4,
	CARD_ANTIALERGIC = 5,
	CARD_SERUM = 6,
	CARD_TRANSPLANT = 7,
	CARD_DORFLEX = 8,
	CARD_MEDICINAL_TEA = 9,
	CARD_FEVER = 10,
	CARD_VOMIT = 101,
	CARD_ANAEMIA = 102,
	CARD_DIARRHEA = 103,
	CARD_RUPTURE = 104
};

typedef struct Card {
	enum CardId id;
	Entity entity;
	const char* title;
	int time_cost;
	int damage;
	int shield;
	int quick;
	int slow;
	int headache;
	int strength;
	int weakness;
	bool disposable;
	bool targetable;
	enum AnimationType animation_type;
} Card;

void card_init(ALLEGRO_BITMAP* small, ALLEGRO_BITMAP* big);

Card create_card(enum CardId id);

Card create_card_from_id(enum CardId id);

void draw_card(Card card);

int calculate_card_x_hand_position(int card_width, int card_number, int hand_max_size);

void position_card(Card* card, int card_number, int hand_max_size);

void highlight_card(Card* card);

#endif