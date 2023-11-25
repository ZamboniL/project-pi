#include "Card.h"
#include "Animation.h"

#ifndef PLAYER_H
#define PLAYER_H

#define DECK_N 20

typedef struct CardCollection {
	Card cards[DECK_N];
	Entity entity;
} CardCollection;

typedef struct Player {
	Animation animation;
	Card hand[5];
	int hand_size;
	CardCollection discard;
	CardCollection deck;
	int next_discard;
	int hand_max_size;
	bool is_dragging_card;
	int card_being_dragged;
	int drag_x_offset;
	int drag_y_offset;
	bool has_highlighted_card;
	int highlighted_card;
	int hovering_arena_entity;
	bool is_hovering_arena_entity;
} Player;

extern Player player;

void update_player();

void shuffle_deck();

void move_discard_to_deck();

void discard_card(int index);

void hand_draw();

void player_init(ALLEGRO_BITMAP* deck, ALLEGRO_BITMAP* discard);

void animate_card_return(int* timer);

void remove_highlighted_card();

void highlight_player_card(int highlight_index);

void draw_player_entities(ALLEGRO_FONT* font);

int player_action();

#endif