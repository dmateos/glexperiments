#include "window.h"

#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>

int window_init(Window *window, int width, int height) {
  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    printf("could not init window subsystem\n");
    exit(1);
  } else {
    printf("initiated SDL video subsystem\n");
  }

  memset(window, 0, sizeof(Window));
  window->width = width;
  window->height = height;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_SetRelativeMouseMode(SDL_TRUE);
  SDL_WarpMouseInWindow(window->window, width / 2, height / 2);

  window->window = SDL_CreateWindow("Test", 0, 0, width, height,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  window->glcontext = SDL_GL_CreateContext(window->window);

  if (SDL_GL_MakeCurrent(window->window, window->glcontext) > 0) {
    printf("could not init window subsystem\n");
    exit(1);
  }

  glClearColor(0, 0, 0.5, 1);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  printf("created SDL window with openGL context\n");
  return 0;
}

int window_destroy(Window *window) {
  SDL_GL_DeleteContext(window->glcontext);
  SDL_DestroyWindow(window->window);
  SDL_Quit();

  printf("destroyed SDL window\n");
  return 0;
}

void window_swap(const Window *window) { SDL_GL_SwapWindow(window->window); }

void window_clear(void) { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

int window_poll(const Window *window, SDL_Event *e) { return SDL_PollEvent(e); }

void window_wireframe(void) {
  static char wireframe = 0;

  if (wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    wireframe = 0;
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    wireframe = 1;
  }
}

int window_get_fps(const Window *window) {
  static unsigned int frame_count, previous_time;
  unsigned int cur_time = SDL_GetTicks();
  char strbuf[1024];
  frame_count++;

  if (cur_time - previous_time >= 1000) {
    snprintf(strbuf, sizeof(strbuf), "%d", frame_count);
    SDL_SetWindowTitle(window->window, strbuf);
    frame_count = 0;
    previous_time = cur_time;
  }

  return 0;
}
