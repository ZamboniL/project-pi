#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include "Animation.h"
#include "CreatureEntity.h"
#include "Card.h"

#ifndef sprintf_s
#define sprintf_s(buf, size, ...) snprintf((buf), size, __VA_ARGS__)
#endif

ALLEGRO_BITMAP* icon_sheet;

void icons_init(ALLEGRO_BITMAP* sheet) {
    icon_sheet = sheet;
}

void draw_creature_entity(CreatureEntity* ce, ALLEGRO_FONT* font) {
    if (ce->health <= 0) {
        return;
    }

    if (ce->entity.is_mouse_over) {
        al_draw_tinted_bitmap_region(
            ce->entity.bitmap,
            al_map_rgb(50, 222, 138),
            ce->animation.curFrame * ce->entity.width,
            ce->animation.animationRow * ce->entity.height,
            ce->entity.width,
            ce->entity.height,
            ce->entity.x,
            ce->entity.y,
            0
        );

    }
    else {
        al_draw_bitmap_region(
            ce->entity.bitmap,
            ce->animation.curFrame * ce->entity.width,
            ce->animation.animationRow * ce->entity.height,
            ce->entity.width,
            ce->entity.height,
            ce->entity.x,
            ce->entity.y,
            0
        );
    }

    char health[8], shield[12];
    sprintf_s(health, 8, "%d/%d", ce->health, ce->max_health);
    sprintf_s(shield, 12, "%d", ce->shield);

    al_draw_filled_rounded_rectangle(
        ce->entity.x,
        ce->entity.y + ce->entity.height + 16,
        ce->entity.x + ce->entity.width,
        ce->entity.y + ce->entity.height + 32,
        8,
        8,
        al_map_rgb(255, 255, 255));

    al_draw_filled_rounded_rectangle(
        ce->entity.x,
        ce->entity.y + ce->entity.height + 16,
        ce->entity.x + (ce->entity.width * ((float)ce->health / ce->max_health)),
        ce->entity.y + ce->entity.height + 32,
        8,
        8,
        al_map_rgb(50, 222, 138));

    al_draw_text(font, al_map_rgb(17, 17, 17), ce->entity.x + ce->entity.width / 2, ce->entity.y + 5 + ce->entity.height + 16, 1, health);

    if (ce->shield) {
        al_draw_bitmap_region(
            icon_sheet,
            0,
            0,
            32,
            32,
            ce->entity.x - 12,
            ce->entity.y + ce->entity.height + 12,
            0
        );
        al_draw_text(font, al_map_rgb(0, 0, 0), ce->entity.x + 4, ce->entity.y + ce->entity.height + 20, 1, shield);
    }

    if (ce->headache) {
        al_draw_bitmap_region(
            icon_sheet,
            32 * 3,
            0,
            32,
            32,
            ce->entity.x,
            ce->entity.y - 32,
            0
        );
        char headache[9];
        sprintf_s(headache, 9, "%d", ce->headache);
        al_draw_text(font, al_map_rgb(255, 255, 255), ce->entity.x + 32 - 8, ce->entity.y - 24, 1, headache);
    }

    if (ce->slowness) {
        al_draw_bitmap_region(
            icon_sheet,
            32 * 2,
            0,
            32,
            32,
            ce->entity.x + 32,
            ce->entity.y - 32,
            0
        );
        char slowness[9];
        sprintf_s(slowness, 9, "%d", ce->slowness);
        al_draw_text(font, al_map_rgb(255, 255, 255), ce->entity.x + 32 * 2 - 8, ce->entity.y - 24, 1, slowness);
    }

    if (ce->strength) {
        al_draw_bitmap_region(
            icon_sheet,
            32 * 4,
            0,
            32,
            32,
            ce->entity.x + 32 * 2,
            ce->entity.y - 32,
            0
        );
        char strength[9];
        sprintf_s(strength, 9, "%d", ce->strength);
        al_draw_text(font, al_map_rgb(255, 255, 255), ce->entity.x + 32 * 3 - 8, ce->entity.y - 24, 1, strength);
    }

    if (ce->weakness) {
        al_draw_bitmap_region(
            icon_sheet,
            32 * 5,
            0,
            32,
            32,
            ce->entity.x + 32 * 3,
            ce->entity.y - 32,
            0
        );
        char weakness[9];
        sprintf_s(weakness, 9, "%d", ce->weakness);
        al_draw_text(font, al_map_rgb(255, 255, 255), ce->entity.x + 32 * 4 - 8, ce->entity.y - 24, 1, weakness);
    }

    if (ce->quickness) {
        al_draw_bitmap_region(
            icon_sheet,
            32 * 1,
            0,
            32,
            32,
            ce->entity.x + 32 * 4,
            ce->entity.y - 32,
            0
        );
        char quickness[9];
        sprintf_s(quickness, 9, "%d", ce->quickness);
        al_draw_text(font, al_map_rgb(255, 255, 255), ce->entity.x + 32 * 5 - 8, ce->entity.y - 24, 1, quickness);
    }

}

void roll_creature_intention(CreatureEntity* ce) {
    ce->play[0] = rand() % ce->possible_plays_quantity;

    if (ce->possible_plays[ce->play[0]].card.time_cost <= ce->max_time_for_next_action && rand() % 100 > 75) {
        int second_play_index = rand() % ce->possible_plays_quantity;
        Play second_play = ce->possible_plays[second_play_index];
        if (second_play.card.time_cost <= ce->max_time_for_next_action) {
            ce->play[1] = second_play_index;
        }
        else {
            ce->play[1] = -1;
        }
    }
}

void change_creature_animation(CreatureEntity* ce, enum AnimationType type) {
    ce->animation = create_animation(type);
    ce->animation.animationRow = ce->animation_coordinates[type][0];
    ce->animation.maxFrame = ce->animation_coordinates[type][1];
}

CreatureEntity create_creature(enum CreatureId id) {
    CreatureEntity creature;
    creature.id = id;
    creature.entity.x = 400;
    creature.entity.y = 166;
    creature.entity.width = 100;
    creature.entity.height = 150;
    creature.last_action_time = 0;
    creature.slowness = 0;
    creature.strength = 0;
    creature.weakness = 0;
    creature.quickness = 0;
    creature.headache = 0;

    return creature;
}

CreatureEntity create_creature_by_id(enum CreatureId id) {
    CreatureEntity creature = create_creature(id);
    switch (id) {
    case CREATURE_ID_CANCER:
        creature.health = 50;
        creature.max_health = 50;
        creature.shield = 10;
        creature.entity.width = 152;
        creature.entity.height = 112;

        Play play1 = { .target = CREATURE_ID_HERO, .card = CARD_VOMIT };
        Play play2 = { .target = CREATURE_ID_HERO, .card = CARD_ANAEMIA };
        Play play3 = { .target = CREATURE_ID_HERO, .card = CARD_DIARRHEA };
        Play play4 = { .target = CREATURE_ID_HERO, .card = CARD_RUPTURE };
        creature.possible_plays[0] = play1;
        creature.possible_plays[1] = play2;
        creature.possible_plays[2] = play3;
        creature.possible_plays[3] = play4;
        creature.max_time_for_next_action = 4 * 60;
        creature.possible_plays_quantity = 4;

        creature.animation_coordinates[IDLE][0] = 0;
        creature.animation_coordinates[IDLE][1] = 7;
        creature.animation_coordinates[ATTACK][0] = 3;
        creature.animation_coordinates[ATTACK][1] = 4;
        creature.animation_coordinates[PROTECT][0] = 1;
        creature.animation_coordinates[PROTECT][1] = 2;
        break;

    case CREATURE_ID_HERO:
        creature.entity.x = 100;
        creature.entity.y = 150;
        creature.entity.width = 128;
        creature.entity.height = 128;
        creature.health = 50;
        creature.shield = 0;
        creature.max_health = 50;

        creature.animation_coordinates[IDLE][0] = 4;
        creature.animation_coordinates[IDLE][1] = 7;
        creature.animation_coordinates[ATTACK][0] = 3;
        creature.animation_coordinates[ATTACK][1] = 3;
        creature.animation_coordinates[PROTECT][0] = 5;
        creature.animation_coordinates[PROTECT][1] = 3;
        creature.animation_coordinates[REST][0] = 6;
        creature.animation_coordinates[REST][1] = 3;
        break;
    }

    change_creature_animation(&creature, IDLE);

    return creature;
}