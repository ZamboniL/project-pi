#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "CombatArena.h"
#include "screen.h"
#include "Entity.h"
#include "Player.h"
#include "Card.h"
#include "CreatureEntity.h"
#include "AnimationTimer.h"
#include "combat.h"

#ifndef sprintf_s
#define sprintf_s(buf, size, ...) snprintf((buf), size, __VA_ARGS__)
#endif

#ifndef strcat_s
#define strcat_s(dest, size, src) strncat((dest), (src), (size))
#endif

void notify_new_turn(const ALLEGRO_FONT *font,char turnText[]) {
    char new_turn_warning[24] = { "Novo turno: " };
    strcat_s(new_turn_warning, sizeof new_turn_warning, turnText);
    al_draw_text(font, al_map_rgb(255, 255, 255), BUFFER_W / 2, 16, ALLEGRO_ALIGN_CENTER, new_turn_warning);
}

int main()
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(DISP_W, DISP_H);
    ALLEGRO_BITMAP* buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    srand(time(NULL));
    bool redraw = true;
    bool done = false;
    ALLEGRO_EVENT event;

    int current_turn = 0;
    int turn = 0;
    int turn_text_animation = 0;

    AnimationTimer animation_timer = { .hand_return = 0 };
    Entity turn_box = { .x = 505, .width = 110, .y = 340, .height = 40, .is_mouse_over = false };

    player_init();
    arena_init();

    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);

        // ESSE � O LOCAL DE EVENTO, TODAS AS INTERA��ES DO USU�RIO FICAM LOCALIZADAS AQUI
        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode != ALLEGRO_KEY_ESCAPE)
                    break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (event.mouse.button == 1) {
                    if (turn_box.is_mouse_over) {
                        turn++;
                    }
                    if (player.has_highlighted_card) {
                        remove_highlighted_card();
                    }
                }
                if (event.mouse.button == 2 && !player.is_dragging_card && !animation_timer.hand_return) {
                    for (int i = 0; i < player.hand_size; i++) {
                        if (player.hand[i].entity.is_mouse_over) {
                            highlight_card(i);
                        }
                    }
                }
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                if (event.mouse.button == 1) {
                    if (player.is_dragging_card) {
                        player.is_dragging_card = false;
                        if (player.is_hovering_arena_entity && player.mana >= player.hand[player.card_being_dragged].cost) {
                            play_card();
                            continue;
                        }

                        animation_timer.hand_return = 60;
                        continue;
                    }

                    if (player.has_highlighted_card || animation_timer.hand_return) {
                        continue;
                    }

                    for (int i = 0; i < player.hand_size; i++) {
                        if (player.hand[i].entity.is_mouse_over) {
                            player.is_dragging_card = true;
                            player.card_being_dragged = i;
                            player.drag_x_offset = event.mouse.x - player.hand[i].entity.x;
                            player.drag_y_offset = event.mouse.y - player.hand[i].entity.y;
                        }
                    }
                }
            case ALLEGRO_EVENT_MOUSE_AXES:
                // Verifica se o mouse est� em cima do her�i
                mark_if_mouse_is_over_arena_entity(event.mouse);
                mark_if_mouse_is_over_entity(event.mouse, &turn_box);

                for (int i = 0; i < player.hand_size; i++) {
                    mark_if_mouse_is_over_entity(event.mouse, &player.hand[i].entity);
                }

                // drag card
                if (player.is_dragging_card) {
                    player.hand[player.card_being_dragged].entity.x = event.mouse.x - player.drag_x_offset;
                    player.hand[player.card_being_dragged].entity.y = event.mouse.y - player.drag_y_offset;
                }

        }

        char turn_text[10];
        sprintf_s(turn_text, sizeof turn_text, "%d", turn);

        if (done) {
            break;
        }

        if (animation_timer.hand_return) {
            animate_card_return(&animation_timer.hand_return);
        }

        if (current_turn != turn) {
            // AQUI FICAM AS CONSEQU�NCIAS DA TROCA DE TURNO
            roll_enemy_intentions();
            turn_text_animation = 360;
            player.mana = player.max_mana;
            hand_draw();
            // ------------------------------------------
            current_turn = turn;
        }


        if (redraw && al_is_event_queue_empty(queue))
        {
            al_set_target_bitmap(buffer);
            al_clear_to_color(al_map_rgb(55, 48, 87));
            // TODO DESENHO A TELA DEVE ACONTECER A PARTIR DAQUI

            if (turn_text_animation > 0) {
                notify_new_turn(font, turn_text);
                turn_text_animation--;
            }

            al_draw_rectangle(turn_box.x, turn_box.y, turn_box.x + turn_box.width, turn_box.y + turn_box.height, al_map_rgb(255, 198, 217), 2);
            al_draw_text(font, al_map_rgb(255, 255, 255), turn_box.x + (turn_box.width / 2), turn_box.y + (turn_box.height / 2) - 4, ALLEGRO_ALIGN_CENTER, "Passar turno");
            
            draw_arena_entities(font);
            draw_player_entities(font);

            // -------------------------------------------------
            al_set_target_backbuffer(disp);
            al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);
            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

