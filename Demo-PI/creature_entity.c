#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "CreatureEntity.h"

void draw_creature_entity(CreatureEntity* ce, ALLEGRO_FONT* font, bool should_display_intention) {
    if (ce->health <= 0) {
        return;
    }
    
    al_draw_filled_rectangle(
        ce->entity.x,
        ce->entity.y,
        ce->entity.x + ce->entity.width,
        ce->entity.y + ce->entity.height,
        ce->entity.is_mouse_over ? al_map_rgb(50, 222, 138) : al_map_rgb(238, 132, 52)
    );

    char health[8], shield[12];
    sprintf_s(health, 8, "HP: %d", ce->health);
    sprintf_s(shield, 12, "Escudo: %d", ce->shield);

    al_draw_text(font, al_map_rgb(255, 255, 255), ce->entity.x, ce->entity.y + ce->entity.height + 16, 0, health);
    al_draw_text(font, al_map_rgb(255, 255, 255), ce->entity.x, ce->entity.y + ce->entity.height + 32, 0, shield);

    if (!should_display_intention) {
        return;
    }

    ALLEGRO_COLOR action_color = al_map_rgb(255, 255, 255);

    switch (ce->intention) {
        case INTENTION_ATTACK:
            action_color = al_map_rgb(238, 132, 52);
            break;
        case INTENTION_SHIELD:
            action_color = al_map_rgb(147, 190, 223);
            break;
        case INTENTION_SPECIAL:
        DEFAULT:
            action_color = al_map_rgb(255, 255, 255);
    }

    char intention_value[2];
    sprintf_s(intention_value, 2, "%d", ce->intention_value);

    al_draw_text(font, action_color, ce->entity.x + (ce->entity.width / 2), ce->entity.y - 16, 1, intention_value);
}

void roll_creature_intention(CreatureEntity* ce) {
    ce->intention = rand() % 3;
    printf("INTENÇÃO %d", ce->intention);
    ce->intention_value = rand() % 5 + 1;
}