#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "Animation.h"
#include "Mouse.h"
#include "Player.h"
#include "Turn.h"
#include "Card.h"
#include "screen.h"
#include "Timeline.h"

#ifndef sprintf_s
#define sprintf_s(buf, size, ...) snprintf((buf), (size), __VA_ARGS__)
#endif

#ifndef strcat_s
#define strcat_s(dest, size, src) strncat((dest), (src), (size))
#endif

Player player;

int card_return = 0;

void update_player() {
	if (turn.is_new_turn) {
		player.animation = create_animation(HAND_DRAW);
	}

	if (player.animation.type == HAND_DRAW) {
		update_animation(&player.animation);

		if (player.animation.curFrame == 9) {
			hand_draw();
		}

		for (int i = 0; i < player.hand_max_size; i++) {
			int position = 0;
			if (player.animation.curFrame <= 9) {
				position = BUFFER_H - (75 - ((75 / 9.0) * player.animation.curFrame));
			}
			else {
				position = BUFFER_H - (((75 / 9.0) * (player.animation.curFrame - 9)));
			}
			player.hand[i].entity.y = position;
		}

		if (player.animation.ended) {
			player.animation.type = IDLE;
		}
	}

	if (mouse_position_updated) {
		for (int i = 0; i < player.hand_max_size; i++) {
			mark_if_mouse_is_over_entity(mouse, &player.hand[i].entity);
		}
		if (player.is_dragging_card) {
			player.hand[player.card_being_dragged].entity.x = mouse.x - player.drag_x_offset;
			player.hand[player.card_being_dragged].entity.y = mouse.y - player.drag_y_offset;
		}
	}

	if (mouse_key[MOUSE_BUTTON_RIGHT] && !player.is_dragging_card) {
		for (int i = 0; i < player.hand_max_size; i++) {
			if (player.hand[i].entity.is_mouse_over) {
				highlight_player_card(i);
			}
		}
	}

	if (mouse_key[MOUSE_BUTTON_LEFT]) {
		if (player.has_highlighted_card) {
			remove_highlighted_card();
		}

		if (!player.has_highlighted_card && !card_return && !player.is_dragging_card) {
			for (int i = 0; i < player.hand_max_size; i++) {
				if (player.hand[i].entity.is_mouse_over) {
					player.is_dragging_card = true;
					player.card_being_dragged = i;
					player.drag_x_offset = mouse.x - player.hand[i].entity.x;
					player.drag_y_offset = mouse.y - player.hand[i].entity.y;
				}
			}
		}
	}

	if (mouse_key[MOUSE_BUTTON_LEFT] == 2) {
		if (player.is_dragging_card) {
			player.is_dragging_card = false;
			if (player.is_hovering_arena_entity) {
				int was_card_played = player_action();
				if (was_card_played) {
					return;
				}
			}
			card_return = 15.0;
		}
	}

	if (card_return) {
		animate_card_return(&card_return);
	}
}

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
	player.hand_size--;

	player.next_discard++;
}

void hand_draw() {
	for (int i = 0; i < player.hand_max_size; i++) {
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

	player.hand_size = player.hand_max_size;
}

void player_init(ALLEGRO_BITMAP* deck_bitmap, ALLEGRO_BITMAP* discard_bitmap)
{
	CardCollection deck;
	deck.entity.x = BUFFER_W - 80;
	deck.entity.width = 40;
	deck.entity.y = 400;
	deck.entity.height = 60;
	deck.entity.bitmap = deck_bitmap;

	CardCollection discard;
	discard.entity.x = 40;
	discard.entity.width = 40;
	discard.entity.y = 400;
	discard.entity.height = 60;
	discard.entity.bitmap = discard_bitmap;

	player.deck = deck;
	player.discard = discard;
	player.card_being_dragged = 0;
	player.is_dragging_card = false;
	player.hand_max_size = 5;
	player.hand_size = 5;
	player.highlighted_card = 0;
	player.has_highlighted_card = false;
	player.next_discard = 0;
	player.drag_x_offset = 0;
	player.drag_y_offset = 0;
	player.animation.type = IDLE;

	for (int i = 0; i < 20; i++) {
		if (i + 1 <= player.hand_max_size) {
			player.hand[i] = create_card_from_id(CARD_NULL);
		}

		player.discard.cards[i] = create_card_from_id(CARD_NULL);

		if (i < 4) {
			player.deck.cards[i] = create_card_from_id(CARD_DORFLEX);
			continue;
		}

		if (i < 8) {
			player.deck.cards[i] = create_card_from_id(CARD_SERUM);
			continue;
		}

		if (i == 9) {
			player.deck.cards[i] = create_card_from_id(CARD_REST);
			continue;
		}

		if (i < 11) {
			player.deck.cards[i] = create_card_from_id(CARD_CAFFEINE);
			continue;
		}

		if (i < 13) {
			player.deck.cards[i] = create_card_from_id(CARD_VACCINE);
			continue;
		}

		if (i == 14) {
			player.deck.cards[i] = create_card_from_id(CARD_FEVER);
			continue;
		}

		if (i == 15) {
			player.deck.cards[i] = create_card_from_id(CARD_MEDICINAL_TEA);
			continue;
		}

		if (i == 16) {
			player.deck.cards[i] = create_card_from_id(CARD_TRANSPLANT);
			continue;
		}

		if (i == 17) {
			player.deck.cards[i] = create_card_from_id(CARD_ANTIALERGIC);
			continue;
		}

		if (i < 20) {
			player.deck.cards[i] = create_card_from_id(CARD_VERMIFUGE);
			continue;
		}
	}

	shuffle_deck();
	hand_draw();
}


void animate_card_return(int* timer) {
	Card* card = &player.hand[player.card_being_dragged];

	int target_x = calculate_card_x_hand_position(card->entity.width, player.card_being_dragged, player.hand_max_size);
	int target_y = BUFFER_H - 75;

	int difference_x = target_x - card->entity.x;
	float speed_x = (float)difference_x / *timer;
	int difference_y = target_y - card->entity.y;
	float speed_y = (float)difference_y / *timer;

	card->entity.x += speed_x;
	card->entity.y += speed_y;
	*timer -= 1;
}

void remove_highlighted_card() {
	Card* prev_highlight = &player.hand[player.highlighted_card];
	prev_highlight->entity.height = 76;
	prev_highlight->entity.width = 50;
	position_card(prev_highlight, player.highlighted_card, player.hand_max_size);
	player.has_highlighted_card = false;
}

void highlight_player_card(int highlight_index) {
	if (player.has_highlighted_card) {
		remove_highlighted_card();
	}

	player.highlighted_card = highlight_index;
	Card* card = &player.hand[highlight_index];
	highlight_card(card);
	player.has_highlighted_card = true;
}

void draw_player_entities(ALLEGRO_FONT* font) {
	int deck_quantity = DECK_N - player.next_discard - player.hand_size;
	int discard_quantity = player.next_discard;
	char deck_display_value[3], discard_display_value[3];

	Entity deck = player.deck.entity;
	Entity discard = player.discard.entity;

	sprintf_s(deck_display_value, 3, "%d", deck_quantity);
	sprintf_s(discard_display_value, 3, "%d", discard_quantity);

	al_draw_bitmap(deck.bitmap, deck.x, deck.y, 0);
	al_draw_filled_circle(deck.x, deck.y + 10, 15, al_map_rgb(147, 190, 223));
	al_draw_text(font, al_map_rgb(255, 255, 255), deck.x, deck.y + 6, 1, deck_display_value);

	al_draw_bitmap(discard.bitmap, discard.x, discard.y, 0);
	al_draw_filled_circle(discard.x, discard.y + 10, 15, al_map_rgb(147, 190, 223));
	al_draw_text(font, al_map_rgb(255, 255, 255), discard.x, discard.y + 6, 1, discard_display_value);

	for (int i = 0; i < player.hand_max_size; i++) {
		draw_card(player.hand[i]);
	}
}

int player_action() {
	Card card = player.hand[player.card_being_dragged];
	int success = add_action_to_timeline(0, player.hovering_arena_entity, card);

	if (success) {
		discard_card(player.card_being_dragged);
	}

	return success;
}