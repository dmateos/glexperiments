#include "window.h"

struct Window *init_window() {
    SDL_Init(SDL_INIT_VIDEO);
    Window *window = malloc(sizeof(Window));
    memset(window, 0, sizeof(Window));

    window->window = SDL_CreateWindow("Test", 0, 0, 1280, 1024,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    window->glcontext = SDL_GL_CreateContext(window->window);

    glClearColor(255, 0, 0, 1);

    return window;
}

int destroy_window(Window *window) {
    SDL_Quit();
    free(window);
    return 0;
}

void swap_window(Window *window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(window->window);
    SDL_Delay(5);
}
