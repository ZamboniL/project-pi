#include "Card.h"

#ifndef TABLE_H
#define TABLE_H

enum {
	HOVERING_HERO = 100
};

typedef struct Table {
	Card hand[5];
	Card discard[15];
	int next_discard;
	Card deck[15];
	int hand_size;
	bool is_dragging_card;
	int card_being_dragged;
	int drag_x_offset;
	int drag_y_offset;
	bool has_highlighted_card;
	int highlighted_card;
	int hovering_arena_entity;
	bool is_hovering_arena_entity;
} Table;

void move_discard_to_deck(Table* table);

int calculate_card_table_x_position(int card_width, int card_number, int hand_size);

void position_card(Card* card, int card_number, int hand_size);

void animate_card_return(Table* table, int* timer);

void remove_highlighted_card(Table* table);

void highlight_card(Table* table, int highlight_index);

void discard_card(Table* table, int hand_index);

void hand_draw(Table* table);

Table create_table();

#endif