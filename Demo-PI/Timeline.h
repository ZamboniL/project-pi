#include "Animation.h"
#include "Entity.h"
#include "Card.h"

#ifndef TIMELINE_H
#define TIMELINE_H

#define MAX_STACK_SIZE 100

enum {
  SOURCE_HERO = 0,
};

typedef struct TimelineCard {
  Entity entity;
  int source;
  bool played;
  int time;
  int target;
  Card card;
} TimelineCard;

typedef struct Timeline {
  Entity entity;
  Entity clock;
  ALLEGRO_BITMAP* arrow;
  Animation animation;
  int size;
  int current;
  int speed;
  bool is_paused;
  TimelineCard stack[MAX_STACK_SIZE];
  int stack_size;
} Timeline;

extern Timeline timeline;

void timeline_init(ALLEGRO_BITMAP* clock, ALLEGRO_BITMAP* arrow, ALLEGRO_BITMAP* frame);
void draw_timeline(ALLEGRO_FONT* font);
void update_timeline();
int add_action_to_timeline(int source, int target, Card card);

#endif