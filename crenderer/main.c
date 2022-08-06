#include "main.h"

#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>
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
  vec3 axis = {1.0, 1.0, 1.0};
  glm_rotate_make(rotated, deg_2_rad(1), axis);

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

int main(int argc, char **argv) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event e;
  ObjFile model;

  Triangle triangle = {
      0, 0, 255, {0.0, 0.0, 0.0}, {0.0, 100.0, 0.0}, {100.0, 100.0, 0.0}};
  Triangle triangle2 = {
      0, 255, 0, {0.0, 0.0, 0.0}, {100.0, 100.0, 0.0}, {100.0, 0.0, 0.0}};

  // parse_obj_file(&model, "monkey.obj");

  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    printf("could not init window subsystem\n");
    exit(1);
  }

  window = SDL_CreateWindow("Test", 0, 0, 1024, 768, SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, -1, 0);

  transform_triangle(&triangle, 300.0, 300.0);
  transform_triangle(&triangle2, 300.0, 300.0);

  print_triangle(&triangle);
  print_triangle(&triangle2);

  while (1) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT:
          exit(0);
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_triangle(renderer, &triangle);
    render_triangle(renderer, &triangle2);

    transform_triangle(&triangle, -350.0, -350.0);
    transform_triangle(&triangle2, -350.0, -350.0);
    rotate_triangle(&triangle, 1);
    rotate_triangle(&triangle2, 1);
    transform_triangle(&triangle, 350.0, 350.0);
    transform_triangle(&triangle2, 350.0, 350.0);

    SDL_RenderPresent(renderer);
    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  return 0;
}