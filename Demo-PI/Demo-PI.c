#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "screen.h"
#include "Entity.h"
#include "Table.h"
#include "Card.h"
#include "CreatureEntity.h"
#include "AnimationTimer.h"


#define TURN_BOX_X 580
#define TURN_BOX_Y 360
#define TURN_BOX_RADIUS 30

//MANA
int mana = 2;
int box_mana_radius = 30;


bool is_mouse_over_turn_box(int mx, int my) {
    int box_max_x = (TURN_BOX_X + TURN_BOX_RADIUS);
    int box_min_x = (TURN_BOX_X - TURN_BOX_RADIUS);
    int box_max_y = (TURN_BOX_Y + TURN_BOX_RADIUS);
    int box_min_y = (TURN_BOX_Y - TURN_BOX_RADIUS);

    return mx >= box_min_x && mx <= box_max_x && my >= box_min_y && my <= box_max_y;
}

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

    CreatureEntity enemy = {.entity.x = 450, .entity.y = 100, .entity.width = 100, .entity.height = 150 };
    roll_creature_intention(&enemy);

    CreatureEntity hero = { .entity.x = 100, .entity.y = 100, .entity.width = 100, .entity.height = 150 };

    Table table = create_table();
    AnimationTimer animation_timer = { .hand_return = 0 };
    
    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);

        // ESSE É O LOCAL DE EVENTO, TODAS AS INTERAÇÕES DO USUÁRIO FICAM LOCALIZADAS AQUI
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
                    if (is_mouse_over_turn_box(event.mouse.x, event.mouse.y)) {
                        turn++;
                    }
                    if (table.has_highlighted_card) {
                        remove_highlighted_card(&table);
                    }
                }
                if (event.mouse.button == 2 && !table.is_dragging_card && !animation_timer.hand_return) {
                    for (int i = 0; i < table.hand_size; i++) {
                        if (table.hand[i].entity.is_mouse_over) {
                            highlight_card(&table, i);
                        }
                    }
                }
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                if (event.mouse.button == 1) {
                    if (table.is_dragging_card) {
                        table.is_dragging_card = false;
                        animation_timer.hand_return = 60;
                        continue;
                    }

                    if (table.has_highlighted_card || animation_timer.hand_return) {
                        continue;
                    }

                    for (int i = 0; i < table.hand_size; i++) {
                        if (table.hand[i].entity.is_mouse_over) {
                            table.is_dragging_card = true;
                            table.card_being_dragged = i;
                            table.drag_x_offset = event.mouse.x - table.hand[i].entity.x;
                            table.drag_y_offset = event.mouse.y - table.hand[i].entity.y;
                        }
                    }
                }
            case ALLEGRO_EVENT_MOUSE_AXES:
                // Verifica se o mouse está em cima do herói
                mark_if_mouse_is_over_entity(event.mouse, &hero.entity);
                // Verifica se o mouse está em cima do inimigo
                mark_if_mouse_is_over_entity(event.mouse, &enemy.entity);

                for (int i = 0; i < table.hand_size; i++) {
                    mark_if_mouse_is_over_entity(event.mouse, &table.hand[i].entity);
                }

                if (table.is_dragging_card) {
                    table.hand[table.card_being_dragged].entity.x = event.mouse.x - table.drag_x_offset;
                    table.hand[table.card_being_dragged].entity.y = event.mouse.y - table.drag_y_offset;
                }

        }

        char turn_text[10];
        sprintf_s(turn_text, sizeof turn_text, "%d", turn);

        if (done) {
            break;
        }

        if (animation_timer.hand_return) {
            animate_card_return(&table, &animation_timer.hand_return);
        }

        if (current_turn != turn) {
            // AQUI FICAM AS CONSEQUÊNCIAS DA TROCA DE TURNO
            roll_creature_intention(&enemy);
            turn_text_animation = 360;
            mana = 2;
            hand_draw(&table);
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

            al_draw_text(font, al_map_rgb(255, 255, 255), TURN_BOX_X + 0, TURN_BOX_Y - 3, ALLEGRO_ALIGN_CENTER, turn_text);
            al_draw_circle(TURN_BOX_X, TURN_BOX_Y, TURN_BOX_RADIUS, al_map_rgb(255, 198, 217), 4);

            //circulo mana
            al_draw_circle(60,360, box_mana_radius, al_map_rgb(147, 190, 223), 4);
            al_draw_text(font, al_map_rgb(255, 255, 255), 60 + 0, 360 - 3, ALLEGRO_ALIGN_CENTER, "2");
            
            draw_creature_entity(&enemy, font, true);
            draw_creature_entity(&hero, font, false);
           

            if (table.has_highlighted_card) {
                al_draw_filled_rectangle(0, 0, DISP_W, DISP_H, al_map_rgba(0, 0, 0, 98));
            }

            for (int i = 0; i < table.hand_size; i++) {
                draw_card(&table.hand[i], font);
            }

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

