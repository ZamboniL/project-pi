#include <allegro5/allegro.h>

#ifndef ANIMATION_H
#define ANIMATION_H

enum AnimationType
{
  IDLE = 0,
  ATTACK = 1,
  PROTECT = 2,
  REST = 3,
  DEFEAT,
  HAND_DRAW,
  CLOCK_TICK,
  SCREEN_TRANSITION
};

typedef struct Animation {
  enum AnimationType  type;
  // Does the animation loop?
  bool loop;
  // Optionally add this if you want to be able to cancel animations.
  // You might for example not want to allow the player
  // to cancel a slide or attack but you do want to allow the player
  // to cancel the idle animation.
  // You could build a far more complex system with some animations
  // allowing to be canceled by a few others but not all.
  bool cancellable;
  // Makes it easier to track whether or not the animation is still playing.
  bool ended;

  int maxFrame;
  int frameCount;
  int animationRow;
  int animationColumns;
  int animationDirection;
  int frameDelay;
  int curFrame;
} Animation;

// Always call this once per frame for each object with an animation.
void update_animation(Animation* animation);

Animation create_animation(enum AnimationType type);

#endif