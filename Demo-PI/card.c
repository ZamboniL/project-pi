#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "Entity.h"
#include "Card.h"

Card create_card(int id, const char* title, int cost, int damage, int shield) {
	Card card;
	card.id = id;
	card.title = title;
	card.cost = cost;
	card.damage = damage;
	card.shield = shield;

	card.entity.width = 50;
	card.entity.height = 75;
	card.entity.is_mouse_over = false;

	return card;
}

Card create_card_from_id(int id) {
	switch (id) {
	case CARD_VACCINE:
		return create_card(1, "Vacina", 1, 0, 4);
	case CARD_ANTIBACTERIAL:
		return create_card(2, "Antibactericida", 1, 8, 0);
	case CARD_NULL:
	default:
		return create_card(0, "Empty", 0, 0, 0);
	}
}

void draw_card(Card* card, ALLEGRO_FONT* font) {
	if (card->id == CARD_NULL) {
		return;
	}

	al_draw_filled_rectangle(card->entity.x, card->entity.y, card->entity.x + card->entity.width, card->entity.y + card->entity.height, al_map_rgb(121, 86, 149));

	char cost[2], damage[2], shield[2];
	sprintf_s(cost, 2, "%d", card->cost);
	sprintf_s(damage, 2, "%d", card->damage);
	sprintf_s(shield, 2, "%d", card->shield);

	al_draw_text(font, al_map_rgb(255, 255, 255), card->entity.x, card->entity.y, 0, cost);
	al_draw_text(font, al_map_rgb(0, 0, 0), card->entity.x + card->entity.width, card->entity.y, ALLEGRO_ALIGN_RIGHT, damage);
	al_draw_text(font, al_map_rgb(59, 255, 91), card->entity.x + card->entity.width, card->entity.y + card->entity.height - 8, ALLEGRO_ALIGN_RIGHT, shield);
}