#include "main.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "camera.h"
#include "model.h"
#include "shader.h"
#include "texture.h"
#include "vertex.h"
#include "window.h"

#define WINDOW_HORIZ 1280
#define WINDOW_VERT 1024
#define MODEL_COUNT 1
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
    pivot_camera(camera, dx, 0);
  }
  if (dy != 0) {
    pivot_camera(camera, 0, dy);
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

  while (poll_window(window, &e)) {
    switch (e.type) {
    case SDL_QUIT:
      return 1;
      break;
    case SDL_KEYDOWN:
      switch (e.key.keysym.sym) {
      case SDLK_a:
        move_camera(camera, CAMERA_RIGHT);
        break;
      case SDLK_d:
        move_camera(camera, CAMERA_LEFT);
        break;
      case SDLK_s:
        move_camera(camera, CAMERA_BACK);
        break;
      case SDLK_w:
        move_camera(camera, CAMERA_FORWARD);
        break;
      case SDLK_RIGHT:
        pivot_camera(camera, 4, 0);
        break;
      case SDLK_LEFT:
        pivot_camera(camera, -4, 0);
        break;
      case SDLK_UP:
        pivot_camera(camera, 0, 4);
        break;
      case SDLK_DOWN:
        pivot_camera(camera, 0, -4);
        break;
      case SDLK_e:
        toggle_wireframe();
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

  init_window(&window, WINDOW_HORIZ, WINDOW_VERT);

  init_shaderprogram(&shader_program);
  add_shader(&shader_program, VERTEXSHADER,
             "shaders/instanced_camera_vert.gsl");
  add_shader(&shader_program, FRAGSHADER, "shaders/frag.gsl");
  compile_shaderprogram(&shader_program);
  use_shaderprogram(&shader_program);

  init_camera(&camera, &shader_program, WINDOW_HORIZ / WINDOW_VERT);
  init_model(&model, &shader_program, argv[1], MODEL_COUNT, model_offsets);
  init_texture(&texture, argv[2]);

  while (!quit) {
    quit = handle_camera(&camera, &window);
    get_window_fps(&window);

    clear_window();

    update_camera(&camera);
    draw_model(&model);
    swap_window(&window);
  }

  destroy_shaderprogram(&shader_program);
  destroy_window(&window);

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

  init_window(&window, WINDOW_HORIZ, WINDOW_VERT);

  init_shaderprogram(&shader_program);
  add_shader(&shader_program, VERTEXSHADER, "shaders/vert.gsl");
  add_shader(&shader_program, FRAGSHADER, "shaders/frag.gsl");
  compile_shaderprogram(&shader_program);
  use_shaderprogram(&shader_program);

  init_camera(&camera, &shader_program, WINDOW_HORIZ / WINDOW_VERT);
  init_texture(&texture, argv[2]);
  model = (Model *)malloc(sizeof(Model) * MODEL_COUNT);

  int col = 0;
  int row = 0;
  int rcount = 0;
  for (int i = 0; i < MODEL_COUNT; i++) {
    init_model(&model[i], &shader_program, argv[1], 0, NULL);
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

    clear_window();

    update_camera(&camera);
    for (int i = 0; i < MODEL_COUNT; i++) {
      draw_model(&model[i]);
    }
    swap_window(&window);
  }

  destroy_shaderprogram(&shader_program);
  destroy_window(&window);
  free(model);
  return 0;
}

int main(int argc, char **argv) {
  int ret = 0;
  if (INSTANCED) {
    ret = main_instanced(argc, argv);
  } else {
    ret = main_normal(argc, argv);
  }
  return ret;
}
