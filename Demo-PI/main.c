#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "screen.h"
#include "game_state.h"
#include "combat.h"
#include "menu.h"

#ifndef sprintf_s
#define sprintf_s(buf, size, ...) snprintf((buf), size, __VA_ARGS__)
#endif

#ifndef strcat_s
#define strcat_s(dest, size, src) strncat((dest), (src), (size))
#endif

int main()
{

    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar allegro.\n");
        return 1;
    }

    if (!al_install_mouse()) {
        fprintf(stderr, "Falha ao inicializar mouse.\n");
        return 1;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao inicializar teclado.\n");
        return 1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Falha ao inicializar addon allegro_primitives.\n");
        return 1;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar addon allegro_image.\n");
        return 1;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(DISP_W, DISP_H);
    ALLEGRO_BITMAP* buffer = al_create_bitmap(BUFFER_W, BUFFER_H);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    srand(time(NULL));

    al_set_window_title(disp, "Adoecido");
    ALLEGRO_EVENT event;


    GameState game_state = create_game_state();

    al_start_timer(timer);
    while (game_state.screen != SCREEN_CLOSE)
    {
        switch (game_state.screen) {
        case SCREEN_START:
            menu(disp, buffer, queue, &game_state);
            break;
        case SCREEN_COMBAT:
        case SCREEN_WIN:
        case SCREEN_DEFEAT:
            combat(disp, buffer, queue, &game_state);
            break;
        }
    }

    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

