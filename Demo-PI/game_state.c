#include "game_state.h"
#include "Animation.h"

GameState create_game_state() {
  GameState game_state;
  game_state.screen = SCREEN_START;

  Animation new_animation;
  new_animation.ended = true;

  game_state.animation = new_animation;

  return game_state;
}

void change_game_state_screen(GameState* game_state, int screen) {
  game_state->screen = screen;

  if (screen == SCREEN_DEFEAT || screen == SCREEN_WIN) {
    Animation new_animation;

    new_animation.curFrame = 0;
    new_animation.frameCount = 0;
    new_animation.frameDelay = 0;
    new_animation.ended = false;
    new_animation.cancellable = false;
    new_animation.loop = false;
    new_animation.maxFrame = 48;

    game_state->animation = new_animation;
  }
}
