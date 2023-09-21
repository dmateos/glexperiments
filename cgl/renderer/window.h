#ifndef _WINDOW_H
#define _WINDOW_H

#include <SDL2/SDL.h>

typedef struct Win {
  SDL_Window *window;
  SDL_GLContext glcontext;
  int mouse_x, mouse_y;
} Window;

int window_init(Window *, int, int);
int window_destroy(Window *);
void window_swap(const Window *);
void window_clear(void);
int window_poll(const Window *, SDL_Event *);
void window_wireframe(void);
int window_get_fps(const Window *);

#endif
