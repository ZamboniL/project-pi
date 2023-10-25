#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>


// resolução do jogo é diferente do tamanho de janela criado, a resolução é definida pelo buffer e o tamanho da janela é o buffer multiplicado
// pelo escalonamento
#define BUFFER_W 640
#define BUFFER_H 480
#define DISP_SCALE 2
#define DISP_W BUFFER_W
#define DISP_H BUFFER_H

#define TURN_BOX_X 580
#define TURN_BOX_Y 360
#define TURN_BOX_RADIUS 30

int hero_x = 450;
int hero_y = 100;
int enemy_x = 100;
int enemy_y = 100;
// quadrado verde
int hero_width = 100;
int hero_height = 150;
// quadrado azul
int enemy_width = 100;
int enemy_height = 150;

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

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(DISP_W, DISP_H);
    ALLEGRO_BITMAP* buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    bool redraw = true;
    bool done = false;
    ALLEGRO_EVENT event;

    int current_turn = 0;
    int turn = 0;
    int turn_text_animation = 0;

    bool is_mouse_over_hero = false;
    bool is_mouse_over_enemy = false;

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
                }
            case ALLEGRO_EVENT_MOUSE_AXES:
                // Verifica se o mouse está em cima do herói
                is_mouse_over_hero = event.mouse.x >= hero_x && event.mouse.x <= hero_x + hero_width &&
                    event.mouse.y >= hero_y && event.mouse.y <= hero_y + hero_height;
                
                // Verifica se o mouse está em cima do inimigo
                is_mouse_over_enemy = event.mouse.x >= enemy_x && event.mouse.x <= enemy_x + enemy_width &&
                    event.mouse.y >= enemy_y && event.mouse.y <= enemy_y + enemy_height;
        }

        char turn_text[10];
        sprintf_s(turn_text, sizeof turn_text, "%d", turn);

        if (done) {
            break;
        }

        if (current_turn != turn) {
            // AQUI FICAM AS CONSEQUÊNCIAS DA TROCA DE TURNO
            turn_text_animation = 360;
            mana = 2;
            // ------------------------------------------
            current_turn = turn;
        }


        if (redraw && al_is_event_queue_empty(queue))
        {
            al_set_target_bitmap(buffer);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            // TODO DESENHO A TELA DEVE ACONTECER A PARTIR DAQUI

            if (turn_text_animation > 0) {
                notify_new_turn(font, turn_text);
                turn_text_animation--;
            }

            al_draw_text(font, al_map_rgb(255, 255, 255), TURN_BOX_X + 0, TURN_BOX_Y - 3, ALLEGRO_ALIGN_CENTER, turn_text);
            al_draw_circle(TURN_BOX_X, TURN_BOX_Y, TURN_BOX_RADIUS, al_map_rgb_f(1, 0, 1), 2);
            al_draw_filled_rectangle(hero_x, hero_y, hero_x + 100, hero_y + 100, al_map_rgb(0, 255, 0));
            al_draw_filled_rectangle(enemy_x, enemy_y, enemy_x + 100, enemy_y + 100, al_map_rgb(0, 0, 255));
            ALLEGRO_COLOR box_color_green = al_map_rgb(0, 255, 0);  
            ALLEGRO_COLOR box_color_blue = al_map_rgb(0, 0, 255);   
            ALLEGRO_COLOR highlight_color = al_map_rgb(255, 255, 0); // Cor de destaque quando o mouse está dentro
            
            //circulo mana
            al_draw_circle(60,360, box_mana_radius, al_map_rgb_f(1, 0, 0), 2);
            al_draw_text(font, al_map_rgb(255, 255, 255), 60 + 0, 360 - 3, ALLEGRO_ALIGN_CENTER,"2");
            
            
            // Verifique a primeira caixa (quadrado verde)
            if (is_mouse_over_hero) {
                // O mouse está dentro da primeira caixa (quadrado verde)
                al_draw_filled_rectangle(hero_x, hero_y, hero_x + hero_width, hero_y + hero_height, highlight_color);
            }
            else {
                // O mouse não está dentro da primeira caixa (quadrado verde)
                al_draw_filled_rectangle(hero_x, hero_y, hero_x + hero_width, hero_y + hero_height, box_color_green);
            }
            // Verifique a segunda caixa (quadrado azul)
            if (is_mouse_over_enemy) {
                // O mouse está dentro da segunda caixa (quadrado azul)
                al_draw_filled_rectangle(enemy_x, enemy_y, enemy_x + enemy_width, enemy_y + enemy_height, highlight_color);
            }
            else {
                // O mouse não está dentro da segunda caixa (quadrado azul)
                al_draw_filled_rectangle(enemy_x, enemy_y, enemy_x + enemy_width, enemy_y + enemy_height, box_color_blue);
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

