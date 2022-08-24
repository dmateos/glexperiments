#include "main.h"
#include "utils.h"

#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define d2r(degrees) degrees *(M_PI / 180)

typedef struct {
  unsigned char r, g, b;
  float p1[3], p2[3], p3[3];
} Triangle;

static void print_triangle(const Triangle *t) {
  printf("p1: %f %f %f\n", t->p1[0], t->p1[1], t->p1[2]);
  printf("p2: %f %f %f\n", t->p2[0], t->p2[1], t->p2[2]);
  printf("p3: %f %f %f\n\n", t->p3[0], t->p3[1], t->p3[2]);
}

static void render_triangle(SDL_Renderer *renderer, const Triangle *t) {
  SDL_SetRenderDrawColor(renderer, t->r, t->g, t->b, 255);
  SDL_RenderDrawLine(renderer, t->p1[0], t->p1[1], t->p2[0], t->p2[1]);
  SDL_RenderDrawLine(renderer, t->p2[0], t->p2[1], t->p3[0], t->p3[1]);
  SDL_RenderDrawLine(renderer, t->p3[0], t->p3[1], t->p1[0], t->p1[1]);
}

static void rotate_triangle(Triangle *t, float degrees, vec3 axis) {
  mat4 rotated;

  glm_rotate_make(rotated, d2r(degrees), axis);
  glm_vec3_rotate_m4(rotated, t->p1, t->p1);
  glm_vec3_rotate_m4(rotated, t->p2, t->p2);
  glm_vec3_rotate_m4(rotated, t->p3, t->p3);
}

static void transform_triangle(Triangle *t, float x, float y) {
  mat4 translate;
  vec3 tc = {x, y, 0.0};

  glm_translate_make(translate, tc);
  glm_mat4_mulv3(translate, t->p1, 1.0, t->p1);
  glm_mat4_mulv3(translate, t->p2, 1.0, t->p2);
  glm_mat4_mulv3(translate, t->p3, 1.0, t->p3);
}

static void scale_triangle(Triangle *t, float s) {
  mat4 scale;
  vec3 tc = {s, s, s};

  glm_scale_make(scale, tc);
  glm_mat4_mulv3(scale, t->p1, 1.0, t->p1);
  glm_mat4_mulv3(scale, t->p2, 1.0, t->p2);
  glm_mat4_mulv3(scale, t->p3, 1.0, t->p3);
}

static Triangle *load_triangles_from_model(const char *file_path, unsigned int *size, float scale, float offsetx, float offsety, vec3 rot_axis) {
  ObjFile *model;
  Triangle *triangles;
  unsigned int vicount_divided;

  model = malloc(sizeof *model);
  memset(model, 0, sizeof(*model));
  parse_obj_file(model, file_path);

  vicount_divided = model->vicount / 3;
  triangles = malloc(sizeof(*triangles) * vicount_divided);

  for (unsigned int i = 0, y = 0; i < vicount_divided; i++, y += 3) {
    triangles[i].r = (unsigned char)255;
    triangles[i].g = (unsigned char)0;
    triangles[i].b = (unsigned char)0;

    triangles[i].p1[0] = model->verticies[model->verticie_index[y] * 3];
    triangles[i].p1[1] = model->verticies[(model->verticie_index[y] * 3) + 1];
    triangles[i].p1[2] = model->verticies[(model->verticie_index[y] * 3) + 2];

    triangles[i].p2[0] = model->verticies[model->verticie_index[y + 1] * 3];
    triangles[i].p2[1] = model->verticies[(model->verticie_index[y + 1] * 3) + 1];
    triangles[i].p2[2] = model->verticies[(model->verticie_index[y + 1] * 3) + 2];

    triangles[i].p3[0] = model->verticies[model->verticie_index[y + 2] * 3];
    triangles[i].p3[1] = model->verticies[(model->verticie_index[y + 2] * 3) + 1];
    triangles[i].p3[2] = model->verticies[(model->verticie_index[y + 2] * 3) + 2];

    rotate_triangle(&triangles[i], 180, rot_axis);
    scale_triangle(&triangles[i], scale);
    transform_triangle(&triangles[i], offsetx, offsety);
  }

  free(model);
  *size = vicount_divided;
  return triangles;
}

int main(int argc, char **argv) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event e;
  Triangle *triangles1, *triangles2, *triangles3;
  unsigned int tcount1, tcount2, tcount3;
  vec3 default_rot = {0.0, 0.0, 1.0};
  vec3 more_rot = {0.0, 1.0, 1.0};

  triangles1 = load_triangles_from_model("monkey.obj", &tcount1, 150, 450, 600, default_rot);
  triangles2 = load_triangles_from_model("torus.obj", &tcount2, 150, 800, 550, default_rot);
  triangles3 = load_triangles_from_model("torus.obj", &tcount3, 150, 800, 550, more_rot);

  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    printf("could not init window subsystem\n");
    exit(1);
  }

  window = SDL_CreateWindow("Test", 0, 0, 1024, 768, SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, -1, 0);

  while (1) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        exit(0);
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (unsigned int i = 0; i < tcount1; i++) {
      render_triangle(renderer, &triangles1[i]);
    }
    for (unsigned int i = 0; i < tcount2; i++) {
      render_triangle(renderer, &triangles2[i]);
    }
    for (unsigned int i = 0; i < tcount3; i++) {
      render_triangle(renderer, &triangles3[i]);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  free(triangles1);
  free(triangles2);
  free(triangles3);
  return 0;
}