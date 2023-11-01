#include <stdio.h>
#include "Table.h"
#include "Card.h"
#include "screen.h"

void shuffle_deck(Card* array, size_t n)
{
	if (n > 1)
	{
		size_t i;
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
}

void move_discard_to_deck(Table* table) {
	for (int i = 0; i < 15; i++) {
		printf("INSERTING CARD: %d %s INTO DECK\n", table->discard[i].id, table->discard[i].title);
		table->deck[i] = create_card_from_id(table->discard[i].id);
		table->discard[i] = create_card_from_id(CARD_NULL);
	}
	table->next_discard = 0;
	shuffle_deck(table->deck, 15);
}

int calculate_card_table_x_position(int card_width, int card_number, int hand_size) {
	if (hand_size % 2 == 0) {
		if (card_number + 1 <= hand_size / 2) {
			if (card_number + 1 - hand_size / 2 == 0) {
				return (BUFFER_W / 2 - card_width) - 12;
			} else {
				return (BUFFER_W / 2 - card_width) - (75 * (hand_size / 2 - card_number - 1)) - 12;
			}
		} else {
			if (card_number - hand_size / 2 == 0) {
				return (BUFFER_W / 2) + 12;
			} else {
				return (BUFFER_W / 2 + 12) + (75 * (hand_size - card_number));
			}
		}

	} else {
		if (card_number + 1 <= hand_size / 2) {
			return (BUFFER_W / 2 - card_width / 2) - (75 * (hand_size / 2 - card_number));
		}
		else if (card_number + 1 == hand_size / 2 + 1) {
			return (BUFFER_W / 2 - card_width / 2);
		}
		else {
			return (BUFFER_W / 2 - card_width / 2) + (75 * (hand_size - card_number));

		}
	}
}

void position_card(Card* card, int card_number, int hand_size) {
	card->entity.x = calculate_card_table_x_position(card->entity.width, card_number, hand_size);
	card->entity.y = BUFFER_H - 75;
	printf("Posicionando carta %d, x: %d, y: %d, hand_size: %d\n", card_number, card->entity.x, card->entity.y, hand_size);
}

void animate_card_return(Table* table, int* timer) {
	Card* card = &table->hand[table->card_being_dragged];

	int target_x = calculate_card_table_x_position(card->entity.width, table->card_being_dragged, table->hand_size);
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

void remove_highlighted_card(Table* table) {
	Card* prev_highlight = &table->hand[table->highlighted_card];
	prev_highlight->entity.height = 75;
	prev_highlight->entity.width = 50;
	position_card(prev_highlight, table->highlighted_card, table->hand_size);
	table->has_highlighted_card = false;
}

void highlight_card(Table* table, int highlight_index) {
	if (table->has_highlighted_card) {
		remove_highlighted_card(table);
	}

	table->highlighted_card = highlight_index;
	Card* card = &table->hand[highlight_index];
	card->entity.width = 200;
	card->entity.height = 300;
	card->entity.x = DISP_W / 2 - card->entity.width / 2;
	card->entity.y = DISP_H / 2 - card->entity.height / 2;
	table->has_highlighted_card = true;
}

void discard_card(Table* table, int hand_index) {
	table->discard[table->next_discard] = create_card_from_id(table->hand[hand_index].id);
	table->hand[hand_index] = create_card_from_id(CARD_NULL);

	table->next_discard++;
}

void hand_draw(Table* table) {
	for (int i = 0; i < table->hand_size; i++) {
		if (table->hand[i].id != CARD_NULL) {
			printf("Descartando carta %d: %d %s, na posição %d do baralho de descarte\n", i, table->hand[i].id, table->hand[i].title, table->next_discard);
			discard_card(table, i);
		}
	}

	for (int i = 0; i < 5; i++) {
		if (table->deck[i].id == CARD_NULL) {
			move_discard_to_deck(table);
		}

		printf("Comprando carta %d: %d %s\n", i, table->deck[i].id, table->deck[i].title);
		table->hand[i] = table->deck[i];

		position_card(&table->hand[i], i, 5);

	}

	for (int i = 0; i < 10; i++) {
		table->deck[i] = create_card_from_id(table->deck[i + 5].id);
		table->deck[i + 5] = create_card_from_id(CARD_NULL);
	}
}

Table create_table() {
	Table table = {
		.hand_size = 5,
		.is_dragging_card = false,
		.card_being_dragged = 0,
		.drag_x_offset = 0,
		.drag_y_offset = 0,
		.highlighted_card = 0,
		.has_highlighted_card = false,
		.next_discard = 0,
		.hovering_arena_entity = 0,
		.is_hovering_arena_entity = false,
		.mana = 2,
		.max_mana = 2,
	};

	table.hand_size = 5;
	table.is_dragging_card = false;

	// FILL TABLE ARRAYS
	for (int i = 0; i < 15; i++) {
		if (i + 1 <= table.hand_size) {
			table.hand[i] = create_card_from_id(CARD_NULL);
		}

		table.discard[i] = create_card_from_id(CARD_NULL);

		printf("Criando carta %d: ", i);

		if (i < 8) {
			table.deck[i] = create_card_from_id(CARD_VACCINE);
			printf("%d %s\n", table.deck[i].id, table.deck[i].title);
			continue;
		}

		table.deck[i] = create_card_from_id(CARD_ANTIBACTERIAL);
		printf("%d %s\n", table.deck[i].id, table.deck[i].title);
	}

	shuffle_deck(table.deck, 15);

	hand_draw(&table);

	return table;
}