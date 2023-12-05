#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "screen.h"
#include "game_state.h"

typedef struct SpriteSheet {
  ALLEGRO_BITMAP* bitmap;
  int width;
  int height;
  int x;
  int y;
  int columns;
  int rows;
  int final_row_columns;
} SpriteSheet;

SpriteSheet background_sheet;

void update_sheet(SpriteSheet* sheet) {
  bool is_in_final_column = sheet->x + sheet->width >= sheet->width * sheet->columns;
  bool is_in_final_row = sheet->y + sheet->height >= sheet->height * sheet->rows;

  if (is_in_final_column) {
    sheet->x = 0;
    if (is_in_final_row) {
      sheet->y = 0;
    }
    else {
      sheet->y += sheet->height;
    }
  }
  else if (is_in_final_row) {
    bool is_in_final_cell = sheet->x + sheet->width >= sheet->width * sheet->final_row_columns;

    if (is_in_final_cell) {
      sheet->x = 0;
      sheet->y = 0;
    }
    else {
      sheet->x += sheet->width;
    }
  }
  else {
    sheet->x += sheet->width;
  }
}

int menu(ALLEGRO_DISPLAY* disp, ALLEGRO_BITMAP* buffer, ALLEGRO_EVENT_QUEUE* event_queue, GameState* game_state) {
  ALLEGRO_EVENT event;
  bool running = true;
  bool redraw = true;

  background_sheet.bitmap = al_load_bitmap("../assets/menu.png");
  background_sheet.width = BUFFER_W;
  background_sheet.height = BUFFER_H;
  background_sheet.x = 0;
  background_sheet.y = 0;
  background_sheet.columns = 9;
  background_sheet.rows = 14;
  background_sheet.final_row_columns = 3;

  al_draw_bitmap_region(
    background_sheet.bitmap,
    background_sheet.x,
    background_sheet.y,
    background_sheet.width,
    background_sheet.height,
    0,
    0,
    0
  );

  while (game_state->screen == SCREEN_START) {
    al_wait_for_event(event_queue, &event);

    switch (event.type)
    {
    case ALLEGRO_EVENT_TIMER:
      redraw = true;
      if (game_state->animation.type == SCREEN_TRANSITION) {
        update_animation(&game_state->animation);
        if (game_state->animation.curFrame >= game_state->animation.maxFrame / 2) {
          change_game_state_screen(game_state, SCREEN_COMBAT);
        }
      }

      update_sheet(&background_sheet);
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
    case ALLEGRO_EVENT_KEY_DOWN:
      game_state->animation = create_animation(SCREEN_TRANSITION);
      break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      game_state->screen = SCREEN_CLOSE;
      break;
    }

    if (redraw && al_is_event_queue_empty(event_queue))
    {
      al_set_target_bitmap(buffer);

      al_draw_bitmap_region(
        background_sheet.bitmap,
        background_sheet.x,
        background_sheet.y,
        background_sheet.width,
        background_sheet.height,
        0,
        0,
        0
      );

      if (game_state->animation.type == SCREEN_TRANSITION) {
        al_draw_filled_rectangle(0, 0, BUFFER_W, BUFFER_H, al_map_rgba(0, 0, 0, 255 * (game_state->animation.curFrame / (float)(game_state->animation.maxFrame / 2))));
      }

      al_set_target_backbuffer(disp);
      al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);
      al_flip_display();

      redraw = false;
    }
  }

  al_destroy_bitmap(background_sheet.bitmap);
}
