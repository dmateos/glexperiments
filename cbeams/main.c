#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "main.h"

#define VERT 1280
#define HORIZ 1280
#define MAPX 8
#define MAPY 8
#define SCREEN_OFFSETX 150

#define TSPEED 0.1

typedef struct {
  float x, y;
} Vec2;

Vec2 player_loc = {1, 1};
Vec2 pd = {1.0, 0.0};

int map[MAPX][MAPY] = {
    {1, 1, 1, 1, 1, 1, 1, 1}, //
    {1, 0, 0, 0, 0, 0, 0, 1}, //
    {1, 0, 1, 0, 1, 1, 0, 1}, //
    {1, 1, 1, 0, 0, 0, 0, 1}, //
    {1, 0, 0, 0, 0, 0, 0, 1}, //
    {1, 0, 0, 0, 0, 0, 0, 1}, //
    {1, 0, 0, 0, 0, 0, 0, 1}, //
    {1, 1, 1, 1, 1, 1, 1, 1}, //
};

void draw_map(SDL_Renderer *renderer) {
  // draw map

  for (int i = 0; i < MAPX; i++) {
    for (int j = 0; j < MAPY; j++) {
      if (map[i][j] > 0) {
        switch (map[i][j]) {
        case 3:
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
          break;
        case 2:
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
          break;
        case 1:
          SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
          break;
        case 4:
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
          break;
        }
        SDL_Rect rect = {j * 16, i * 16, 16, 16};
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }

  // draw player
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_Rect rect = {player_loc.x * 16, player_loc.y * 16, 4, 4};
  SDL_RenderFillRect(renderer, &rect);

  // draw camera direction
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawLine(renderer, player_loc.x * 16, player_loc.y * 16,
                     player_loc.x * 16 + pd.x * 100,
                     player_loc.y * 16 + pd.y * 100);
}

void draw_vert_line(SDL_Renderer *renderer, int x, int start, int end, int c) {
  switch (c) {
  case 1:
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    break;
  case 2:
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    break;
  case 3:
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    break;
  }
  SDL_Rect rect = {x * 10, start + SCREEN_OFFSETX, 10, end - start};
  SDL_RenderFillRect(renderer, &rect);
}

int main(int argc, char **argv) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event e;
  int cont = 1;
  float old_x;

  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    printf("could not init window subsystem\n");
    exit(1);
  }

  window = SDL_CreateWindow("Test", 0, 0, VERT, HORIZ, SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, -1, 0);

  while (cont) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        cont = 0;
      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
        case SDLK_w:
          player_loc.x = player_loc.x + pd.x * 0.1;
          player_loc.y = player_loc.y + pd.y * 0.1;
          break;
        case SDLK_s:
          player_loc.x = -player_loc.x + pd.x * 0.1;
          player_loc.y = -player_loc.y + pd.y * 0.1;
          break;
        case SDLK_a:
          old_x = pd.x;
          pd.x = old_x * cos(TSPEED) + pd.y * sin(TSPEED);
          pd.y = -old_x * sin(TSPEED) + pd.y * cos(TSPEED);
          printf("x: %f, y: %f\n", pd.x, pd.y);
          break;
        case SDLK_d:
          // turn right
          old_x = pd.x;
          pd.x = old_x * cos(-TSPEED) + pd.y * sin(-TSPEED);
          pd.y = -old_x * sin(-TSPEED) + pd.y * cos(-TSPEED);
          printf("x: %f, y: %f\n", pd.x, pd.y);
          break;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    draw_map(renderer);

    // convert player pos to map pos
    int mapPosX = (int)(player_loc.y);
    int mapPosY = (int)(player_loc.x);
    printf("mapPosX: %d, mapPosY: %d\n", mapPosX, mapPosY);
    printf("player_loc.x: %f, player_loc.y: %f\n", player_loc.x, player_loc.y);

    if (map[mapPosX][mapPosY] > 0) {
      map[mapPosX][mapPosY] = 4;
      printf("mapPosX: %d, mapPosY: %d\n", mapPosX, mapPosY);
      printf("hit\n");
    }

    for (int i = 0; i < 100; i++) {
      draw_vert_line(renderer, i, 0, 100, 1);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  return 0;
}
