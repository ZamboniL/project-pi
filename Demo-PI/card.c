#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Animation.h"
#include "Entity.h"
#include "Card.h"
#include "screen.h"

#ifndef sprintf_s
#define sprintf_s(buf, size, ...) snprintf((buf), size, __VA_ARGS__)
#endif

ALLEGRO_BITMAP* small_sprite_sheet;
ALLEGRO_BITMAP* big_sprite_sheet;

void card_init(ALLEGRO_BITMAP* small, ALLEGRO_BITMAP* big) {
	small_sprite_sheet = small;
	big_sprite_sheet = big;
}


Card create_card(enum CardId id) {
	Card card;
	card.id = id;
	card.title = "Empty";
	card.time_cost = 0;
	card.damage = 0;
	card.shield = 0;
	card.quick = 0;
	card.slow = 0;
	card.headache = 0;
	card.strength = 0;
	card.weakness = 0;
	card.disposable = false;
	card.targetable = true;
	card.animation_type = ATTACK;


	card.entity.width = 50;
	card.entity.height = 76;
	card.entity.is_mouse_over = false;

	return card;
}

Card create_card_from_id(enum CardId id) {
	Card card = create_card(id);
	switch (id) {
	case CARD_VACCINE:
		card.title = "Vacina";
		card.time_cost = 7 * 60;
		card.damage = 0;
		card.shield = 16;
		card.animation_type = PROTECT;
		card.targetable = false;
		card.entity.sheet_column = 1;
		card.entity.sheet_row = 2;
		return card;
	case CARD_VERMIFUGE:
		card.title = "Vermifugo";
		card.time_cost = 5 * 60;
		card.damage = 8;
		card.weakness = 2;
		card.shield = 0;
		card.animation_type = ATTACK;
		card.entity.sheet_column = 0;
		card.entity.sheet_row = 2;
		return card;
	case CARD_REST:
		card.title = "Repouso";
		card.time_cost = 12 * 60;
		card.damage = 0;
		card.shield = 0;
		card.animation_type = REST;
		card.targetable = false;
		card.entity.sheet_column = 0;
		card.entity.sheet_row = 0;
		return card;
	case CARD_CAFFEINE:
		card.title = "Cafeína";
		card.time_cost = 0;
		card.damage = 0;
		card.shield = 0;
		card.animation_type = PROTECT;
		card.quick = 1;
		card.targetable = false;
		card.entity.sheet_column = 1;
		card.entity.sheet_row = 0;
		return card;
	case CARD_ANTIALERGIC:
		card.title = "Antialérgico";
		card.time_cost = 4 * 60;
		card.damage = 0;
		card.shield = 16;
		card.animation_type = PROTECT;
		card.slow = 1;
		card.targetable = false;
		card.entity.sheet_column = 3;
		card.entity.sheet_row = 1;
		return card;
	case CARD_SERUM:
		card.title = "Soro";
		card.time_cost = 4 * 60 + 30;
		card.damage = 0;
		card.shield = 8;
		card.animation_type = PROTECT;
		card.targetable = false;
		card.entity.sheet_column = 2;
		card.entity.sheet_row = 0;
		return card;
	case CARD_TRANSPLANT:
		card.title = "Transplante";
		card.time_cost = 6 * 60;
		card.damage = 10;
		card.shield = 0;
		card.animation_type = ATTACK;
		card.entity.sheet_column = 3;
		card.entity.sheet_row = 0;
		return card;
	case CARD_DORFLEX:
		card.title = "Dorflex";
		card.time_cost = 4 * 60 + 30;
		card.damage = 8;
		card.shield = 0;
		card.animation_type = ATTACK;
		card.entity.sheet_column = 0;
		card.entity.sheet_row = 1;
		return card;
	case CARD_MEDICINAL_TEA:
		card.title = "Chá Medicinal";
		card.time_cost = 3 * 60;
		card.damage = 0;
		card.shield = 0;
		card.animation_type = PROTECT;
		card.weakness = 3;
		card.entity.sheet_column = 1;
		card.entity.sheet_row = 1;
		return card;
	case CARD_FEVER:
		card.title = "Febre";
		card.time_cost = 6 * 60;
		card.damage = 16;
		card.shield = 0;
		card.animation_type = ATTACK;
		card.headache = 2;
		card.entity.sheet_column = 2;
		card.entity.sheet_row = 1;
		return card;
	case CARD_VOMIT:
		card.title = "Vomito";
		card.time_cost = 5 * 60;
		card.damage = 8;
		card.shield = 2;
		card.weakness = 2;
		card.animation_type = PROTECT;
		card.entity.sheet_column = 2;
		card.entity.sheet_row = 2;
		return card;
	case CARD_ANAEMIA:
		card.title = "Anemia";
		card.time_cost = 6 * 60;
		card.damage = 8;
		card.headache = 2;
		card.shield = 0;
		card.animation_type = ATTACK;
		card.entity.sheet_column = 3;
		card.entity.sheet_row = 2;
		return card;
	case CARD_DIARRHEA:
		card.title = "Diarréia";
		card.time_cost = 8 * 60;
		card.damage = 16;
		card.shield = 0;
		card.slow = 2;
		card.animation_type = ATTACK;
		card.entity.sheet_column = 0;
		card.entity.sheet_row = 3;
		return card;
	case CARD_RUPTURE:
		card.title = "Ruptura";
		card.time_cost = 4 * 60;
		card.damage = 0;
		card.shield = 0;
		card.animation_type = ATTACK;
		card.entity.sheet_column = 1;
		card.entity.sheet_row = 3;
		return card;
	case CARD_NULL:
	default:
		return card;
	}
}

void draw_card(Card card) {
	if (card.id == CARD_NULL) {
		return;
	}

	if (card.entity.height == 76) {
		al_draw_bitmap_region(
			small_sprite_sheet,
			card.entity.sheet_column * card.entity.width,
			card.entity.sheet_row * card.entity.height,
			card.entity.width,
			card.entity.height,
			card.entity.x,
			card.entity.y,
			0
		);
	}
	else {
		al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba(0, 0, 0, 98));
		al_draw_bitmap_region(
			big_sprite_sheet,
			card.entity.sheet_column * card.entity.width,
			card.entity.sheet_row * card.entity.height,
			card.entity.width,
			card.entity.height,
			card.entity.x,
			card.entity.y,
			0
		);
	}
}


int calculate_card_x_hand_position(int card_width, int card_number, int hand_max_size) {
	if (hand_max_size % 2 == 0) {
		if (card_number + 1 <= hand_max_size / 2) {
			if (card_number + 1 - hand_max_size / 2 == 0) {
				return (BUFFER_W / 2 - card_width) - 12;
			}
			else {
				return (BUFFER_W / 2 - card_width) - (75 * (hand_max_size / 2 - card_number - 1)) - 12;
			}
		}
		else {
			if (card_number - hand_max_size / 2 == 0) {
				return (BUFFER_W / 2) + 12;
			}
			else {
				return (BUFFER_W / 2 + 12) + (75 * (hand_max_size - card_number));
			}
		}

	}
	else {
		if (card_number + 1 <= hand_max_size / 2) {
			return (BUFFER_W / 2 - card_width / 2) - (75 * (hand_max_size / 2 - card_number));
		}
		else if (card_number + 1 == hand_max_size / 2 + 1) {
			return (BUFFER_W / 2 - card_width / 2);
		}
		else {
			return (BUFFER_W / 2 - card_width / 2) + (75 * (hand_max_size - card_number));

		}
	}
}

void position_card(Card* card, int card_number, int hand_max_size) {
	card->entity.x = calculate_card_x_hand_position(card->entity.width, card_number, hand_max_size);
	card->entity.y = BUFFER_H - 75;
}


void highlight_card(Card* card) {
	card->entity.width = 150;
	card->entity.height = 228;
	card->entity.x = BUFFER_W / 2 - card->entity.width / 2;
	card->entity.y = BUFFER_H / 2 - card->entity.height / 2;
}