#ifndef _WINDOW_H
#define _WINDOW_H

#include <OpenGL/gl.h>
#include <SDL2/SDL.h>

typedef struct Window {
    SDL_Window *window;
    SDL_GLContext glcontext;
} Window;

struct Window *init_window();
int destroy_window(Window *window);
void swap_window(const Window *window);
int poll_window(const Window *window);

#endif