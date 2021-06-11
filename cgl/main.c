#include "main.h"

#include <stdbool.h>
#include <stdio.h>

#include "window.h"

int main() {
    Window *window = init_window(1280, 1024);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        swap_window(window);
    }

    destroy_window(window);
    return 0;
}