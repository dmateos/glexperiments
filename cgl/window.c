#include "window.h"

#include <OpenGL/gl3.h>

int init_window(Window *window, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        printf("could not init window subsystem\n");
        exit(1);
    } else {
        printf("initiated SDL video subsystem\n");
    }

    memset(window, 0, sizeof(Window));

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    window->window = SDL_CreateWindow("Test", 0, 0, width, height,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    window->glcontext = SDL_GL_CreateContext(window->window);

    if (SDL_GL_MakeCurrent(window->window, window->glcontext) > 0) {
        printf("could not init window subsystem\n");
        exit(1);
    }

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    printf("created SDL window with openGL context\n");
    return 0;
}

int destroy_window(Window *window) {
    SDL_GL_DeleteContext(window->glcontext);
    SDL_DestroyWindow(window->window);
    SDL_Quit();

    printf("destroyed SDL window\n");
    return 0;
}

void swap_window(const Window *window) {
    SDL_GL_SwapWindow(window->window);
    SDL_Delay(5);
}

void clear_window() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

int poll_window(const Window *window, SDL_Event *e) { return SDL_PollEvent(e); }
