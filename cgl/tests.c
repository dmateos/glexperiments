#include "base/list.h"
#include "renderer/camera.h"
#include "renderer/model.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
#include "renderer/window.h"

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

int test_instanced(Window *window, int mc, int argc, char **argv) {
  ShaderProgram shader_program;
  Model model;
  ModelSkybox skybox;
  Camera camera;
  bool quit = false;
  float *model_offsets = malloc(sizeof(float) * mc * 3);
  memset(model_offsets, 0, sizeof(float) * mc * 3);

  if (argc < 3) {
    printf("specify models to load\n");
    exit(1);
  }

  float col = 0.0;
  float row = 0.0;
  int rcount = 0;

  for (int i = 0; i < mc * 3; i += 3) {
    model_offsets[i] += row;
    // model_offsets[i + 1] += ((float)rand() / (float)(RAND_MAX)) * 1000.0;
    model_offsets[i + 1] = 0.0;
    model_offsets[i + 2] += col;

    row += 2.0;
    rcount += 1.0;

    if ((rcount % (int)sqrt(mc) == 0) && (rcount != 0)) {
      row = 0.0;
      col += 2.0;
      rcount = 0;
    }
  }

  model_skybox_init(&skybox, "assets/textures/skybox/");
  model_shader_init(&shader_program, 1);

  shader_use(&shader_program);
  camera_init(&camera, window->width / window->height);
  model_init(&model, &shader_program, argv[1], argv[2], mc, model_offsets);

  while (!quit) {
    quit = handle_camera(&camera, window);
    window_get_fps(window);

    window_clear();

    camera_update(&camera, &shader_program, 0);
    camera_update(&camera, &skybox.program, 1);

    model_skybox_draw(&skybox);
    model_draw(&model);

    window_swap(window);
  }

  model_free(&model);
  free(model_offsets);
  return 0;
}

int test_normal(Window *window, int mc, int argc, char **argv) {
  ShaderProgram shader_program;
  Model *model;
  ModelSkybox skybox;
  Camera camera;
  bool quit = false;

  if (argc < 3) {
    printf("specify models to load\n");
    exit(1);
  }

  model_skybox_init(&skybox, "assets/textures/skybox/");
  model_shader_init(&shader_program, 0);

  shader_use(&shader_program);
  camera_init(&camera, window->width / window->height);
  model = (Model *)malloc(sizeof(Model) * mc);

  int col = 0;
  int row = 0;
  int rcount = 0;
  for (int i = 0; i < mc; i++) {
    model_init(&model[i], &shader_program, argv[1], argv[2], 0, NULL);
    model[i].vec[2] += row;
    model[i].vec[0] += col;

    row += 3;
    rcount += 1;

    if ((rcount % (int)sqrt(mc) == 0) && (rcount != 0)) {
      row = 0;
      col += 3;
      rcount = 0;
    }
  }

  while (!quit) {
    quit = handle_camera(&camera, window);
    window_get_fps(window);

    window_clear();

    camera_update(&camera, &shader_program, 0);
    camera_update(&camera, &skybox.program, 1);
    model_skybox_draw(&skybox);
    for (int i = 0; i < mc; i++) {
      model_draw(&model[i]);
    }
    window_swap(window);
  }

  for (int i = 0; i < mc; i++) {
    model_free(&model[i]);
  }
  free(model);
  return 0;
}