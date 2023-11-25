#include <stdio.h>
#include "CreatureEntity.h"
#include "Animation.h"
#include "Card.h"

void heal_entity(CreatureEntity* target, int heal) {
  if (target->health + heal > target->max_health) {
    target->health = target->max_health;
    return;
  }

  target->health += heal;
}

void shield_entity(CreatureEntity* target, int shield) {
  target->shield += shield;
}

void damage_entity(CreatureEntity* source, CreatureEntity* target, int damage) {
  if (source->weakness > 0) {
    damage *= 0.6;
  }

  if (!target->shield) {
    target->health -= damage;
  }
  else if (damage >= target->shield) {
    int difference = target->shield - damage;
    target->health += difference;
    target->shield = 0;
  }
  else if (damage) {
    target->shield -= damage;
  }
}

void play_headache_card(CreatureEntity* source, CreatureEntity* target, Card card) {
  target->headache += card.headache;
}

void play_weakness_card(CreatureEntity* source, CreatureEntity* target, Card card) {
  target->weakness += card.weakness;
}

void play_slowness_card(CreatureEntity* source, CreatureEntity* target, Card card) {
  target->slowness += card.slow;
}

void play_quickness_card(CreatureEntity* source, CreatureEntity* target, Card card) {
  source->quickness += card.quick;
}

void play_rest_card(CreatureEntity* source, CreatureEntity* target) {
  heal_entity(target, target->max_health / 2);
  target->weakness = 0;
  target->slowness = 0;
  target->headache = 0;
}

void play_shield_card(CreatureEntity* source, CreatureEntity* target, Card card) {
  shield_entity(target, card.shield);
}

void play_damage_card(CreatureEntity* source, CreatureEntity* target, Card card) {
  damage_entity(source, target, card.damage);
}

void play_transplant_card(CreatureEntity* source, CreatureEntity* target, Card card) {
  int damage = card.damage;

  if (target->health <= target->max_health * 0.2) {
    damage *= 2;
  }

  damage_entity(source, target, damage);
}

void play_rupture_card(CreatureEntity* source, CreatureEntity* target, Card card) {
  int shield = target->shield;
  target->shield = 0;
  source->shield += shield;
}

void activate_card(CreatureEntity* source, CreatureEntity* target, Card card) {
  change_creature_animation(source, card.animation_type);
  switch (card.id) {
  case CARD_REST:
    return play_rest_card(source, target);
  case CARD_VOMIT:
    play_damage_card(source, target, card);
    play_shield_card(source, source, card);
    return play_weakness_card(source, target, card);
  case CARD_CAFFEINE:
    return play_quickness_card(source, target, card);
  case CARD_MEDICINAL_TEA:
    return play_weakness_card(source, target, card);
  case CARD_ANTIALERGIC:
    play_slowness_card(source, source, card);
    return play_shield_card(source, source, card);
  case CARD_DIARRHEA:
    play_damage_card(source, target, card);
    return play_slowness_card(source, target, card);
  case CARD_RUPTURE:
    return play_rupture_card(source, target, card);
  case CARD_SERUM:
  case CARD_VACCINE:
    return play_shield_card(source, target, card);
  case CARD_ANAEMIA:
    play_damage_card(source, target, card);
    return play_headache_card(source, target, card);
  case CARD_TRANSPLANT:
    return play_transplant_card(source, target, card);
  case CARD_FEVER:
    play_headache_card(source, source, card);
    return play_damage_card(source, target, card);
  case CARD_DORFLEX:
    return play_damage_card(source, target, card);
  case CARD_VERMIFUGE:
    play_weakness_card(source, target, card);
    return play_damage_card(source, target, card);
  default:
    return;
  }
}
