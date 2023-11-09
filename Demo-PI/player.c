#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "Player.h"
#include "Card.h"
#include "screen.h"

void shuffle_deck()
{
	Card* array = player.deck.cards;
	size_t n = DECK_N, i;

	for (i = 0; i < n - 1; i++)
	{
		if (array[i].id == CARD_NULL) {
			return;
		}

		size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
		Card t = array[j];
		while (t.id == CARD_NULL) {
			j = i + rand() / (RAND_MAX / (n - i) + 1);
			t = array[j];
		}

		array[j] = array[i];
		array[i] = t;
	}
}

void move_discard_to_deck() {
	for (int i = 0; i < 15; i++) {
		player.deck.cards[i] = create_card_from_id(player.discard.cards[i].id);
		player.discard.cards[i] = create_card_from_id(CARD_NULL);
	}

	player.next_discard = 0;
	shuffle_deck();
}

void discard_card(int index) {
	player.discard.cards[player.next_discard] = create_card_from_id(player.hand[index].id);
	player.hand[index] = create_card_from_id(CARD_NULL);

	player.next_discard++;
}

void hand_draw() {
	for (int i = 0; i < player.hand_size; i++) {
		if (player.hand[i].id != CARD_NULL) {
			discard_card(i);
		}
	}

	for (int i = 0; i < 5; i++) {
		if (player.deck.cards[i].id == CARD_NULL) {
			move_discard_to_deck(player);
		}

		player.hand[i] = player.deck.cards[i];
		position_card(&player.hand[i], i, 5);
	}

	for (int i = 0; i < 10; i++) {
		player.deck.cards[i] = create_card_from_id(player.deck.cards[i + 5].id);
		player.deck.cards[i + 5] = create_card_from_id(CARD_NULL);
	}
}

void player_init()
{
	CardCollection deck;
	deck.entity.x = DISP_W - 80;
	deck.entity.width = 40;
	deck.entity.y = 410;
	deck.entity.height = 60;

	CardCollection discard;
	discard.entity.x = 40;
	discard.entity.width = 40;
	discard.entity.y = 410;
	discard.entity.height = 60;

	player.deck = deck;
	player.discard = discard;
	player.card_being_dragged = 0;
	player.is_dragging_card = false;
	player.hand_size = 5;
	player.highlighted_card = 0;
	player.has_highlighted_card = false;
	player.mana = 2;
	player.max_mana = 2;
	player.next_discard = 0;
	player.drag_x_offset = 0;
	player.drag_y_offset = 0;

	for (int i = 0; i < 15; i++) {
		if (i + 1 <= player.hand_size) {
			player.hand[i] = create_card_from_id(CARD_NULL);
		}

		player.discard.cards[i] = create_card_from_id(CARD_NULL);

		if (i < 8) {
			player.deck.cards[i] = create_card_from_id(CARD_VACCINE);
			continue;
		}

		player.deck.cards[i] = create_card_from_id(CARD_ANTIBACTERIAL);
	}

	shuffle_deck();
	hand_draw();
}


void animate_card_return(int* timer) {
	Card* card = &player.hand[player.card_being_dragged];

	int target_x = calculate_card_x_hand_position(card->entity.width, player.card_being_dragged, player.hand_size);
	int target_y = BUFFER_H - 75;
	int direction = 1;

	int difference_x = target_x - card->entity.x;
	int speed_x = difference_x / *timer;
	int difference_y = target_y - card->entity.y;
	int speed_y = difference_y / *timer;

	card->entity.x = card->entity.x + speed_x;
	card->entity.y = card->entity.y + speed_y;
	*timer -= 1;
}

void remove_highlighted_card() {
	Card* prev_highlight = &player.hand[player.highlighted_card];
	prev_highlight->entity.height = 75;
	prev_highlight->entity.width = 50;
	position_card(prev_highlight, player.highlighted_card, player.hand_size);
	player.has_highlighted_card = false;
}

void highlight_card(int highlight_index) {
	if (player.has_highlighted_card) {
		remove_highlighted_card();
	}

	player.highlighted_card = highlight_index;
	Card* card = &player.hand[highlight_index];
	card->entity.width = 200;
	card->entity.height = 300;
	card->entity.x = DISP_W / 2 - card->entity.width / 2;
	card->entity.y = DISP_H / 2 - card->entity.height / 2;
	player.has_highlighted_card = true;
}

void draw_player_entities(ALLEGRO_FONT* font) {
	int deck_quantity = DECK_N - player.next_discard - player.hand_size;
	int discard_quantity = player.next_discard;
	char* deck_display_value[3], discard_display_value[3], mana_display_value[3];

	Entity deck = player.deck.entity;
	Entity discard = player.discard.entity;

	sprintf_s(deck_display_value, 3, "%d", deck_quantity);
	sprintf_s(discard_display_value, 3, "%d", discard_quantity);
	sprintf_s(mana_display_value, 3, "%d", player.mana);

	al_draw_circle(60, 360, MANA_CIRCLE_RADIUS, al_map_rgb(147, 190, 223), 4);
	al_draw_text(font, al_map_rgb(255, 255, 255), 60 + 2, 360 - 3, ALLEGRO_ALIGN_CENTER, mana_display_value);

	al_draw_filled_rounded_rectangle(deck.x, deck.y, deck.x + deck.width, deck.y + deck.height, 4, 4, al_map_rgb(255, 255, 255));
	al_draw_filled_circle(deck.x, deck.y + 10, 15, al_map_rgb(147, 190, 223), 4);
	al_draw_text(font, al_map_rgb(255, 255, 255), deck.x, deck.y + 6, 1, deck_display_value);

	al_draw_filled_rounded_rectangle(discard.x, discard.y, discard.x + discard.width, discard.y + discard.height, 4, 4, al_map_rgb(255,255,255));
	al_draw_filled_circle(discard.x, discard.y + 10, 15, al_map_rgb(147, 190, 223), 4);
	al_draw_text(font, al_map_rgb(255, 255, 255), discard.x, discard.y + 6, 1, discard_display_value);


	if (player.has_highlighted_card) {
		al_draw_filled_rectangle(0, 0, DISP_W, DISP_H, al_map_rgba(0, 0, 0, 98));
	}

	for (int i = 0; i < player.hand_size; i++) {
		draw_card(player.hand[i], font);
	}
}