#include "window.h"

Window *init_window(int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);
    Window *window = malloc(sizeof(Window));
    memset(window, 0, sizeof(Window));

    window->window = SDL_CreateWindow("Test", 0, 0, width, height,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    window->glcontext = SDL_GL_CreateContext(window->window);

    glClearColor(255, 0, 0, 1);

    return window;
}

int destroy_window(Window *window) {
    SDL_GL_DeleteContext(window->glcontext);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
    free(window);
    return 0;
}

void swap_window(const Window *window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(window->window);
    SDL_Delay(5);
}

int poll_window(const Window *window) { return 0; }
