#include "Animation.h"

#ifndef GAME_STATE_H
#define GAME_STATE_H


enum Screen {
  SCREEN_START = 0,
  SCREEN_COMBAT = 1,
  SCREEN_WIN = 2,
  SCREEN_DEFEAT = 3,
  SCREEN_CLOSE = 4,
  SCREEN_TUTORIAL = 5,
};

typedef struct GameState {
  int screen;
  Animation animation;
} GameState;

GameState create_game_state();

void change_game_state_screen(GameState* game_state, int screen);

#endif