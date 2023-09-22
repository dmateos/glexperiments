#include "main.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "base/list.h"
#include "renderer/camera.h"
#include "renderer/model.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
#include "renderer/window.h"
#include "tests.h"

#define WINDOW_HORIZ 1280
#define WINDOW_VERT 1024
#define INSTANCED 0

// handle mouse, do it like a blender 3d app where you click to drag
// and right click to rotate
static void handle_mouse(Camera *camera, Window *window) {
  int x, y;
  int dx, dy;
  int button_state = SDL_GetMouseState(&x, &y);
  dx = (x - window->mouse_x);
  dy = (y - window->mouse_y);

  if (dx != 0) {
    camera_pivot(camera, dx, 0);
  }
  if (dy != 0) {
    camera_pivot(camera, 0, -dy);
  }

  if (button_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
  }

  if (button_state & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
  }

  window->mouse_x = x;
  window->mouse_y = y;
}

static int handle_camera(Camera *camera, Window *window) {
  SDL_Event e;

  handle_mouse(camera, window);

  while (window_poll(window, &e)) {
    switch (e.type) {
    case SDL_QUIT:
      return 1;
      break;
    case SDL_KEYDOWN:
      switch (e.key.keysym.sym) {
      case SDLK_a:
        camera_move(camera, CAMERA_RIGHT);
        break;
      case SDLK_d:
        camera_move(camera, CAMERA_LEFT);
        break;
      case SDLK_s:
        camera_move(camera, CAMERA_BACK);
        break;
      case SDLK_w:
        camera_move(camera, CAMERA_FORWARD);
        break;
      case SDLK_RIGHT:
        camera_pivot(camera, 4, 0);
        break;
      case SDLK_LEFT:
        camera_pivot(camera, -4, 0);
        break;
      case SDLK_UP:
        camera_pivot(camera, 0, 4);
        break;
      case SDLK_DOWN:
        camera_pivot(camera, 0, -4);
        break;
      case SDLK_e:
        window_wireframe();
        break;
      }
      break;
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  Window window;
  int ret = 0;

  list_test();

  window_init(&window, WINDOW_HORIZ, WINDOW_VERT);

  if (INSTANCED) {
    ret = test_instanced(&window, argc, argv);
  } else {
    ret = test_normal(&window, argc, argv);
  }
  return ret;
}
