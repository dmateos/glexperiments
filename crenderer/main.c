#include "main.h"

#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

typedef struct _triangle {
  char r, g, b;
  float p1[3], p2[3], p3[3];
} Triangle;

static float deg_2_rad(float degrees) { return degrees * (3.1415 / 180); }

static void print_triangle(Triangle *t) {
  printf("p1: %f %f %f\n", t->p1[0], t->p1[1], t->p1[2]);
  printf("p2: %f %f %f\n", t->p2[0], t->p2[1], t->p2[2]);
  printf("p3: %f %f %f\n\n", t->p3[0], t->p3[1], t->p3[2]);
}

static void render_triangle(SDL_Renderer *renderer, Triangle *t) {
  SDL_SetRenderDrawColor(renderer, t->r, t->g, t->b, 255);
  SDL_RenderDrawLine(renderer, t->p1[0], t->p1[1], t->p2[0], t->p2[1]);
  SDL_RenderDrawLine(renderer, t->p2[0], t->p2[1], t->p3[0], t->p3[1]);
  SDL_RenderDrawLine(renderer, t->p3[0], t->p3[1], t->p1[0], t->p1[1]);
}

static void rotate_triangle(Triangle *t, float degrees) {
  mat4 rotated;
  vec3 axis = {0.0, 0.0, 1.0};
  glm_rotate_make(rotated, deg_2_rad(degrees), axis);

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

int main(int argc, char **argv) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event e;
  ObjFile *model;
  Triangle *triangles;

  model = malloc(sizeof *model);
  parse_obj_file(model, "monkey.obj");
  triangles = malloc(sizeof(*triangles) * model->vicount);

  for (unsigned int i = 0; i < model->vicount; i++) {
    triangles[i].r = 255;

    triangles[i].p1[0] = model->verticies[model->verticie_index[i] * 3];
    triangles[i].p1[1] = model->verticies[(model->verticie_index[i] * 3) + 1];
    triangles[i].p1[2] = model->verticies[(model->verticie_index[i] * 3) + 2];

    triangles[i].p2[0] = model->verticies[model->verticie_index[i + 1] * 3];
    triangles[i].p2[1] =
        model->verticies[(model->verticie_index[i + 1] * 3) + 1];
    triangles[i].p2[2] =
        model->verticies[(model->verticie_index[i + 1] * 3) + 2];

    triangles[i].p3[0] = model->verticies[model->verticie_index[i + 2] * 3];
    triangles[i].p3[1] =
        model->verticies[(model->verticie_index[i + 2] * 3) + 1];
    triangles[i].p3[2] =
        model->verticies[(model->verticie_index[i + 2] * 3) + 2];

    rotate_triangle(&triangles[i], 180);
    scale_triangle(&triangles[i], 200.0);
    transform_triangle(&triangles[i], 600, 600);
  }

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

    printf("%d\n", model->vicount);
    for (unsigned int i = 0; i < model->vicount / 3; i++) {
      print_triangle(&triangles[i]);
      render_triangle(renderer, &triangles[i]);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  free(model);
  free(triangles);
  return 0;
}