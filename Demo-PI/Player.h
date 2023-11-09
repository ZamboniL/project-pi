#include "Card.h"

#ifndef PLAYER_H
#define PLAYER_H

#define DECK_N 15
#define MANA_CIRCLE_RADIUS 30

enum {
	HOVERING_HERO = 100
};

typedef struct CardCollection {
	Card cards[DECK_N];
	Entity entity;
} CardCollection;

typedef struct Player {
	Card hand[5];
	CardCollection discard;
	CardCollection deck;
	int next_discard;
	int hand_size;
	bool is_dragging_card;
	int card_being_dragged;
	int drag_x_offset;
	int drag_y_offset;
	bool has_highlighted_card;
	int highlighted_card;
	int hovering_arena_entity;
	bool is_hovering_arena_entity;
	int mana;
	int max_mana;
} Player;

extern Player player;

void shuffle_deck();

void move_discard_to_deck();

void discard_card(int index);

void hand_draw();

void player_init();

void animate_card_return(int* timer);

void remove_highlighted_card();

void highlight_card(int highlight_index);

void draw_player_entities(ALLEGRO_FONT* font);

#endif