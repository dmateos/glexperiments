#include "main.h"

#include <OpenGL/gl.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

struct Window {
    SDL_Window *window;
    SDL_GLContext glcontext;
};

struct Window *init_window() {
    SDL_Init(SDL_INIT_VIDEO);
    struct Window *window = malloc(sizeof(struct Window));

    window->window = SDL_CreateWindow("Test", 0, 0, 1280, 1024,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    window->glcontext = SDL_GL_CreateContext(window->window);

    glClearColor(255, 0, 0, 1);

    return window;
}

int destroy_window(struct Window *window) {
    SDL_Quit();
    free(window);
    return 0;
}

int main() {
    struct Window *window = init_window();

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SDL_GL_SwapWindow(window->window);
        SDL_Delay(5);
    }

    destroy_window(window);
    return 0;
}