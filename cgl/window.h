#ifndef _WINDOW_H
#define _WINDOW_H

#include <SDL2/SDL.h>

typedef struct Win {
    SDL_Window *window;
    SDL_GLContext glcontext;
} Window;

int init_window(Window *, int, int);
int destroy_window(Window *);
void swap_window(const Window *);
int poll_window(const Window *, SDL_Event *);
void clear_window(void);

#endif