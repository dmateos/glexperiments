#include "main.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "renderer/camera.h"
#include "renderer/model.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
#include "renderer/vertex.h"
#include "renderer/window.h"

#include "base/list.h"

#define WINDOW_HORIZ 1280
#define WINDOW_VERT 1024
#define MODEL_COUNT 1000000
#define INSTANCED 1

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
    camera_pivot(camera, 0, dy);
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

int main_instanced(int argc, char **argv) {
  Window window;
  ShaderProgram shader_program;
  Model model;
  Camera camera;
  Texture texture;
  bool quit = false;
  float model_offsets[MODEL_COUNT * 3];

  if (argc < 3) {
    printf("specify models to load\n");
    exit(1);
  }

  int col = 0;
  int row = 0;
  int rcount = 0;
  for (int i = 0; i < MODEL_COUNT * 3; i += 3) {
    model_offsets[i] += row;
    model_offsets[i + 1] += ((float)rand() / (float)(RAND_MAX)) * 1000.0;
    model_offsets[i + 2] += col;

    row += 10;
    rcount += 1;

    if ((rcount % (int)sqrt(MODEL_COUNT) == 0) && (rcount != 0)) {
      row = 0;
      col += 10;
      rcount = 0;
    }
  }

  window_init(&window, WINDOW_HORIZ, WINDOW_VERT);

  shader_program_init(&shader_program);
  shader_program_add(&shader_program, VERTEXSHADER,
                     "renderer/shaders/instanced_camera_vert.gsl");
  shader_program_add(&shader_program, FRAGSHADER, "renderer/shaders/frag.gsl");
  shader_program_compile(&shader_program);
  shader_use(&shader_program);

  camera_init(&camera, &shader_program, WINDOW_HORIZ / WINDOW_VERT);
  model_init(&model, &shader_program, argv[1], MODEL_COUNT, model_offsets);
  texture_init(&texture, argv[2]);

  while (!quit) {
    quit = handle_camera(&camera, &window);
    window_get_fps(&window);

    window_clear();

    camera_update(&camera);
    model_draw(&model);
    window_swap(&window);
  }

  shader_program_destroy(&shader_program);
  window_destroy(&window);

  return 0;
}

int main_normal(int argc, char **argv) {
  Window window;
  ShaderProgram shader_program;
  Model *model;
  Camera camera;
  Texture texture;
  bool quit = false;

  if (argc < 3) {
    printf("specify models to load\n");
    exit(1);
  }

  window_init(&window, WINDOW_HORIZ, WINDOW_VERT);

  shader_program_init(&shader_program);
  shader_program_add(&shader_program, VERTEXSHADER,
                     "renderer/shaders/vert.gsl");
  shader_program_add(&shader_program, FRAGSHADER, "renderer/shaders/frag.gsl");
  shader_program_compile(&shader_program);
  shader_use(&shader_program);

  camera_init(&camera, &shader_program, WINDOW_HORIZ / WINDOW_VERT);
  texture_init(&texture, argv[2]);
  model = (Model *)malloc(sizeof(Model) * MODEL_COUNT);

  int col = 0;
  int row = 0;
  int rcount = 0;
  for (int i = 0; i < MODEL_COUNT; i++) {
    model_init(&model[i], &shader_program, argv[1], 0, NULL);
    model[i].vec[2] += row;
    model[i].vec[0] += col;

    row += 3;
    rcount += 1;

    if ((rcount % (int)sqrt(MODEL_COUNT) == 0) && (rcount != 0)) {
      row = 0;
      col += 3;
      rcount = 0;
    }
  }

  while (!quit) {
    quit = handle_camera(&camera, &window);

    window_clear();

    camera_update(&camera);
    for (int i = 0; i < MODEL_COUNT; i++) {
      model_draw(&model[i]);
    }
    window_swap(&window);
  }

  shader_program_destroy(&shader_program);
  window_destroy(&window);
  free(model);
  return 0;
}

int main(int argc, char **argv) {
  int ret = 0;

  list_test();
  return 0;

  if (INSTANCED) {
    ret = main_instanced(argc, argv);
  } else {
    ret = main_normal(argc, argv);
  }
  return ret;
}
