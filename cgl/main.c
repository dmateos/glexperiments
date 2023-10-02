#include "main.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "base/list.h"
#include "renderer/camera.h"
#include "renderer/model.h"
#include "renderer/scene.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
#include "renderer/window.h"
#include "tests.h"

#define WINDOW_HORIZ 2560
#define WINDOW_VERT 1536

#define TEST 1
#define INSTANCED 1
#define MODEL_COUNT 1000000

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

static int handle_scene(Window *window) {
  Scene scene;
  bool quit = false;

  scene_init(&scene, WINDOW_HORIZ / WINDOW_VERT);

  while (!quit) {
    quit = handle_camera(&scene.camera, window);
    window_clear();

    scene_update(&scene);
    scene_draw(&scene);
    window_swap(window);
  }

  scene_free(&scene);
  return 0;
}

int main(int argc, char **argv) {
  Window window;
  int ret = 0;

  window_init(&window, WINDOW_HORIZ, WINDOW_VERT);

  if (TEST) {
    list_test();
    if (INSTANCED) {
      ret = test_instanced(&window, MODEL_COUNT, argc, argv);
    } else {
      ret = test_normal(&window, MODEL_COUNT, argc, argv);
    }
  } else {
    ret = handle_scene(&window);
  }

  window_destroy(&window);
  return ret;
}
