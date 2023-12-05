#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include "game_state.h"
#include "screen.h"
#include "Turn.h"
#include "CombatArena.h"
#include "Player.h"
#include "Timeline.h"
#include "Mouse.h"
#include "Animation.h"


int combat(ALLEGRO_DISPLAY* disp, ALLEGRO_BITMAP* buffer, ALLEGRO_EVENT_QUEUE* event_queue, GameState* game_state) {
  ALLEGRO_BITMAP* background;
  ALLEGRO_BITMAP* timeline_clock;
  ALLEGRO_BITMAP* timeline_arrow;
  ALLEGRO_BITMAP* timeline_frame;
  ALLEGRO_BITMAP* hero;
  ALLEGRO_BITMAP* enemy;
  ALLEGRO_BITMAP* deck;
  ALLEGRO_BITMAP* discard;
  ALLEGRO_BITMAP* icon_sheet;
  ALLEGRO_BITMAP* card_small;
  ALLEGRO_BITMAP* card_big;
  ALLEGRO_BITMAP* turn_button;
  ALLEGRO_BITMAP* tutorial_background;
  ALLEGRO_BITMAP* tutorial_icon;
  background = al_load_bitmap("../assets/background.png");
  timeline_clock = al_load_bitmap("../assets/clock.png");
  timeline_arrow = al_load_bitmap("../assets/arrow.png");
  timeline_frame = al_load_bitmap("../assets/timeline_frame.png");
  hero = al_load_bitmap("../assets/hero.png");
  enemy = al_load_bitmap("../assets/stomach_worm.png");
  deck = al_load_bitmap("../assets/deck.png");
  discard = al_load_bitmap("../assets/discard.png");
  icon_sheet = al_load_bitmap("../assets/icon-sheet.png");
  card_small = al_load_bitmap("../assets/card-sheet-1x.png");
  card_big = al_load_bitmap("../assets/card-sheet-4x.png");
  turn_button = al_load_bitmap("../assets/turn.png");
  tutorial_background = al_load_bitmap("../assets/tutorial.png");
  tutorial_icon = al_load_bitmap("../assets/tutorial-icon.png");


  ALLEGRO_FONT* font = al_create_builtin_font();
  ALLEGRO_EVENT event;

  Entity tutorial;
  tutorial.bitmap = tutorial_icon;
  tutorial.x = DISP_W - 32;
  tutorial.y = 0;
  tutorial.width = 32;
  tutorial.height = 25;
  tutorial.is_mouse_over = false;

  bool running = true;
  bool redraw = true;

  icons_init(icon_sheet);
  card_init(card_small, card_big);
  turn_init(turn_button);
  player_init(deck, discard);
  arena_init(hero, enemy);
  timeline_init(timeline_clock, timeline_arrow, timeline_frame);
  mouse_init();

  al_draw_bitmap(background, 0, 0, 0);

  while (
    game_state->screen == SCREEN_COMBAT ||
    game_state->screen == SCREEN_WIN ||
    game_state->screen == SCREEN_DEFEAT ||
    game_state->screen == SCREEN_TUTORIAL
    ) {
    al_wait_for_event(event_queue, &event);

    switch (event.type)
    {
    case ALLEGRO_EVENT_TIMER:
      redraw = true;
      if (game_state->screen == SCREEN_WIN || game_state->screen == SCREEN_DEFEAT) {
        update_animation(&game_state->animation);
        break;
      }

      if (game_state->animation.type == SCREEN_TRANSITION) {
        update_animation(&game_state->animation);
      }

      update_turn();
      update_arena(game_state);
      update_player();
      update_timeline();
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
      if (mark_if_mouse_is_over_entity(event.mouse, &tutorial)) {
        if (game_state->screen == SCREEN_TUTORIAL) {
          change_game_state_screen(game_state, SCREEN_COMBAT);
          tutorial.x = DISP_W - 32;
          tutorial.y = 0;
          tutorial.width = 32;
          tutorial.height = 25;
        }
        else {
          change_game_state_screen(game_state, SCREEN_TUTORIAL);
          tutorial.x = 580;
          tutorial.y = 2;
          tutorial.width = 58;
          tutorial.height = 50;
        }

        break;
      }
      if (game_state->screen == SCREEN_DEFEAT || game_state->screen == SCREEN_WIN) {
        change_game_state_screen(game_state, SCREEN_START);
        break;
      }
      break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      game_state->screen = SCREEN_CLOSE;
      break;
    }

    if (!running) {
      break;
    }

    update_mouse(event);

    if (turn.is_new_turn) {
      roll_enemy_intentions();
      timeline.is_paused = turn.value == 1;
    }

    if (redraw && al_is_event_queue_empty(event_queue))
    {
      al_set_target_bitmap(buffer);
      al_clear_to_color(al_map_rgb(55, 48, 87));
      al_draw_bitmap(background, 0, 0, 0);


      draw_turn(font);
      draw_arena_entities(font);
      draw_timeline(font);
      draw_player_entities(font);

      al_draw_bitmap(tutorial_icon, DISP_W - 32, 0, 0);


      if (game_state->animation.type == SCREEN_TRANSITION && !game_state->animation.ended) {
        al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba(0, 0, 0, 255 * (1 - (game_state->animation.curFrame % 25 / (float)(game_state->animation.maxFrame / 2)))));
      }

      float screen_alpha = ((game_state->animation.curFrame * 0.9) / game_state->animation.maxFrame) * 255;
      float text_y = (game_state->animation.curFrame * (BUFFER_H / 2)) / game_state->animation.maxFrame;

      switch (game_state->screen) {
      case SCREEN_TUTORIAL:
        al_draw_bitmap(tutorial_background, 0, 0, 0);
        break;
      case SCREEN_DEFEAT:
        al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba(204, 25, 38, screen_alpha));
        al_draw_text(font, al_map_rgb(255, 255, 255), BUFFER_W / 2, text_y, 1, "VOCE PERDEU!");
        al_draw_text(font, al_map_rgb(255, 255, 255), BUFFER_W / 2, text_y + 20, 1, "TENTAR NOVAMENTE?");
        break;
      case SCREEN_WIN:
        al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba(85, 115, 255, screen_alpha));
        al_draw_text(font, al_map_rgb(255, 255, 255), BUFFER_W / 2, text_y, 1, "VOCE VENCEU!");
        al_draw_text(font, al_map_rgb(255, 255, 255), BUFFER_W / 2, text_y + 20, 1, "JOGAR NOVAMENTE?");
        break;
      }

      al_set_target_backbuffer(disp);
      al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);
      al_flip_display();

      redraw = false;
    }
  }

  al_destroy_font(font);
  al_destroy_bitmap(background);
  al_destroy_bitmap(timeline_clock);
  al_destroy_bitmap(timeline_arrow);
  al_destroy_bitmap(timeline_frame);
  al_destroy_bitmap(hero);
  al_destroy_bitmap(enemy);
  al_destroy_bitmap(deck);
  al_destroy_bitmap(discard);
  al_destroy_bitmap(icon_sheet);
  al_destroy_bitmap(card_small);
  al_destroy_bitmap(card_big);
  al_destroy_bitmap(turn_button);
  al_destroy_bitmap(tutorial_icon);
  al_destroy_bitmap(tutorial_background);
}
