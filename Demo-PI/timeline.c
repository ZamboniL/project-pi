#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "Mouse.h"
#include "Animation.h"
#include "Timeline.h"
#include "screen.h"
#include "Card.h"
#include "CombatArena.h"
#include "CreatureEntity.h"
#include "CombatController.h"

Timeline timeline;

void timeline_init(ALLEGRO_BITMAP* clock, ALLEGRO_BITMAP* arrow, ALLEGRO_BITMAP* frame)
{
  timeline.current = 0;
  timeline.size = 720;
  timeline.speed = 5;
  timeline.entity.x = 64;
  timeline.entity.width = BUFFER_W - 100;
  timeline.entity.y = 24;
  timeline.entity.height = 32;
  timeline.entity.bitmap = frame;
  timeline.arrow = arrow;
  timeline.clock.x = 24;
  timeline.clock.y = 16;
  timeline.clock.height = 48;
  timeline.clock.width = 48;
  timeline.clock.bitmap = clock;
  timeline.is_paused = true;
  timeline.stack_size = 0;
  timeline.animation = create_animation(CLOCK_TICK);
}

void draw_timeline(ALLEGRO_FONT* font)
{
  al_draw_bitmap_region(
    timeline.entity.bitmap,
    0,
    0,
    timeline.entity.width,
    timeline.entity.height,
    timeline.entity.x,
    timeline.entity.y,
    0
  );

  for (int i = 0; i < timeline.stack_size; i++) {
    TimelineCard current = timeline.stack[i];
    ALLEGRO_COLOR action_color;

    if (current.played) {
      continue;
    }

    if (current.source == CREATURE_ID_HERO) {
      action_color = al_map_rgb(85, 115, 174);
    }
    else {
      action_color = al_map_rgb(179, 27, 27);

    }

    int card_width = current.entity.x + current.entity.width / 2 > timeline.entity.x + timeline.entity.width
      ? timeline.entity.x + timeline.entity.width
      : current.entity.x + current.entity.width / 2;

    al_draw_filled_rectangle(
      current.entity.x - current.entity.width / 2,
      current.entity.y,
      card_width,
      current.entity.y + current.entity.height,
      action_color
    );

    if (current.entity.is_mouse_over) {
      draw_card(current.card);
    }
  }

  al_draw_bitmap_region(
    timeline.entity.bitmap,
    0,
    timeline.entity.height,
    timeline.entity.width,
    timeline.entity.height,
    timeline.entity.x,
    timeline.entity.y,
    0
  );

  al_draw_bitmap(
    timeline.arrow,
    timeline.entity.x + (timeline.current * ((float)timeline.entity.width / timeline.size)) - 8,
    timeline.entity.y - 4,
    0
  );

  al_draw_bitmap_region(
    timeline.clock.bitmap,
    (timeline.animation.curFrame) * timeline.clock.width,
    0,
    timeline.clock.width,
    timeline.clock.height,
    timeline.clock.x,
    timeline.clock.y,
    0
  );
}

void activate_action(TimelineCard* action) {
  CreatureEntity* source = get_creature_by_id(action->source);
  CreatureEntity* target = get_creature_by_id(action->target);
  action->played = true;

  activate_card(source, target, action->card);
}

void play_stack(int interval_min, int interval_max) {
  for (int i = 0; i < timeline.stack_size; i++) {
    TimelineCard* current = &timeline.stack[i];
    CreatureEntity* source = get_creature_by_id(current->source);


    if (current->played) {
      continue;
    }

    source->last_action_time = 0;

    if (current->time >= interval_min && current->time <= interval_max) {
      activate_action(current);
    }
  }
}


void clear_timeline() {
  timeline.stack_size = 0;
}

void update_timeline() {
  for (int i = 0; i < timeline.stack_size; i++) {
    TimelineCard* current = &timeline.stack[i];
    mark_if_mouse_is_over_entity(mouse, &current->entity);
  }

  if (timeline.is_paused) {
    if (!timeline.stack_size) {
      for (int i = 0; i < arena.enemies_quantity; i++) {
        CreatureEntity* enemy = &arena.enemies[i];
        if (enemy->health > 0) {
          Play play = enemy->possible_plays[enemy->play[0]];
          Card card = create_card_from_id(play.card.id);
          add_action_to_timeline(enemy->id, play.target, card);

          if (enemy->play[1] != -1) {
            Play play = enemy->possible_plays[enemy->play[1]];
            Card card = create_card_from_id(play.card.id);
            add_action_to_timeline(enemy->id, play.target, card);
          }

          break;
        }
      }
    }
    return;
  }

  update_animation(&timeline.animation);

  if (timeline.current + timeline.speed <= timeline.size) {
    int last_time = timeline.current;
    timeline.current += timeline.speed;
    play_stack(last_time + 1, timeline.current);
  }
  else {
    timeline.current = 0;
    timeline.is_paused = true;
    clear_timeline();
  }
}

int add_action_to_timeline(int source, int target, Card card) {
  if (!timeline.is_paused) {
    return 0;
  }

  CreatureEntity* source_creature = get_creature_by_id(source);

  int execution_time = card.time_cost + source_creature->last_action_time;

  if (source_creature->slowness) {
    execution_time += 120;
  }

  if (source_creature->quickness) {
    execution_time -= 120;
  }

  if (execution_time > timeline.size) {
    return 0;
  }

  source_creature->last_action_time = execution_time;

  if (timeline.stack_size >= MAX_STACK_SIZE) {
    return 0;
  }

  if (card.time_cost == 0) {
    activate_card(source_creature, get_creature_by_id(target), card);
    return 1;
  }

  float x = timeline.entity.x + (execution_time * ((float)timeline.entity.width / timeline.size));

  TimelineCard timeline_card;
  timeline_card.entity.x = x;
  timeline_card.entity.width = 32;
  timeline_card.entity.y = 24;
  timeline_card.entity.height = 32;

  timeline_card.card = card;
  highlight_card(&timeline_card.card);

  timeline_card.played = false;
  timeline_card.source = source;
  timeline_card.target = target;
  timeline_card.time = execution_time;
  timeline.stack[timeline.stack_size] = timeline_card;
  timeline.stack_size++;

  return 1;
}


