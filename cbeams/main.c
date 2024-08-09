#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "main.h"

#define VERT 800
#define HORIZ 600
#define MAPX 8
#define MAPY 8
#define SCREEN_OFFSETX 150
#define RECTSIZE 16
#define TSPEED 0.10
#define w 640
#define h 480

typedef struct {
  double x, y;
} Vec2;

typedef struct hit {
  int colour;
  double dist;
  int side;
} Hit;

Vec2 player_loc = {2, 2};
Vec2 pd = {1.0, 0.0};
Vec2 cp = {0.0, 0.66};

int map[MAPX][MAPY] = {
    {1, 2, 1, 2, 1, 2, 1, 1}, //
    {2, 0, 0, 0, 0, 0, 0, 1}, //
    {1, 0, 1, 0, 1, 1, 0, 1}, //
    {2, 1, 2, 0, 0, 0, 0, 1}, //
    {2, 0, 1, 1, 2, 1, 0, 2}, //
    {1, 0, 1, 0, 0, 0, 0, 1}, //
    {1, 0, 0, 0, 0, 0, 0, 1}, //
    {1, 1, 1, 2, 1, 1, 1, 1}, //
};

void draw_map(SDL_Renderer *renderer) {
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
        SDL_Rect rect = {j * RECTSIZE, i * RECTSIZE, RECTSIZE, RECTSIZE};
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }

  // draw player
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_Rect rect = {player_loc.x * RECTSIZE, player_loc.y * RECTSIZE, 4, 4};
  SDL_RenderFillRect(renderer, &rect);

  // draw camera direction
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawLine(renderer, player_loc.x * RECTSIZE, player_loc.y * RECTSIZE,
                     player_loc.x * RECTSIZE + pd.x * 100,
                     player_loc.y * RECTSIZE + pd.y * 100);
}

void draw_vert_line(SDL_Renderer *renderer, int x, int start, int end, int c,
                    int side) {
  switch (c) {
  case 1:
    if (side == 0) {
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, 255 / 2, 0, 0, 255);
    }
    break;
  case 2:
    if (side == 0) {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, 0, 255 / 2, 0, 255);
    }
    break;
  case 3:
    if (side == 0) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, 0, 0, 255 / 2, 255);
    }
    break;
  }

  SDL_RenderDrawLine(renderer, x, start + SCREEN_OFFSETX, x,
                     end + SCREEN_OFFSETX);
}

Hit walk_squares_to_find_hit(double rayDirX, double rayDirY) {
  // convert player pos to map pos
  int mapPosX = (int)(player_loc.x);
  int mapPosY = (int)(player_loc.y);
  int hit = 0;
  int stepX, stepY, side;
  double distX, distY = 0;
  double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
  double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
  double perpWallDist;

  if (rayDirX < 0) {
    stepX = -1;
    distX = (player_loc.x - mapPosX) * deltaDistX;
  } else {
    stepX = 1;
    distX = (mapPosX + 1.0 - player_loc.x) * deltaDistX;
  }

  if (rayDirY < 0) {
    stepY = -1;
    distY = (player_loc.y - mapPosY) * deltaDistY;
  } else {
    stepY = 1;
    distY = (mapPosY + 1.0 - player_loc.y) * deltaDistY;
  }

  while (!hit) {
    if (distX < distY) {
      distX += deltaDistX;
      mapPosX += stepX;
      hit = map[mapPosX][mapPosY];
      side = 0;
    } else {
      distY += deltaDistY;
      mapPosY += stepY;
      hit = map[mapPosX][mapPosY];
      side = 1;
    }
  }

  if (side == 0) {
    perpWallDist = (distX - deltaDistX);
  } else {
    perpWallDist = (distY - deltaDistY);
  }
  Hit x = {.colour = map[mapPosX][mapPosY], .dist = perpWallDist, .side = side};
  return x;
}

int main(int argc, char **argv) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event e;
  int cont = 1;
  double old_x, new_x, new_y;

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
        break;
      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
        case SDLK_w:
          new_x = player_loc.x + pd.x * 0.1;
          new_y = player_loc.y + pd.y * 0.1;

          // if (map[(int)new_x][(int)player_loc.y] == 0) {
          player_loc.x = player_loc.x + pd.x * 0.1;
          //}
          // if (map[(int)player_loc.x][(int)new_y] == 0) {
          player_loc.y = player_loc.y + pd.y * 0.1;
          //}
          break;
        case SDLK_s:
          new_x = player_loc.x + pd.x * 0.1;
          new_y = player_loc.y + pd.y * 0.1;

          // if (map[(int)new_x][(int)player_loc.y] == 0) {
          player_loc.x = player_loc.x + pd.x * -0.1;
          //}
          // if (map[(int)player_loc.x][(int)new_y] == 0) {
          player_loc.y = player_loc.y + pd.y * -0.1;
          //}
          break;
        case SDLK_a:
          old_x = pd.x;
          pd.x = old_x * cos(TSPEED) + pd.y * sin(TSPEED);
          pd.y = -old_x * sin(TSPEED) + pd.y * cos(TSPEED);
          old_x = cp.x;
          cp.x = old_x * cos(TSPEED) + cp.y * sin(TSPEED);
          cp.y = -old_x * sin(TSPEED) + cp.y * cos(TSPEED);
          break;
        case SDLK_d:
          // turn right
          old_x = pd.x;
          pd.x = old_x * cos(-TSPEED) + pd.y * sin(-TSPEED);
          pd.y = -old_x * sin(-TSPEED) + pd.y * cos(-TSPEED);
          old_x = cp.x;
          cp.x = old_x * cos(-TSPEED) + cp.y * sin(-TSPEED);
          cp.y = -old_x * sin(-TSPEED) + cp.y * cos(-TSPEED);
          break;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    draw_map(renderer);

    // convert player pos to map pos
    int mapPosX = (int)(player_loc.x);
    int mapPosY = (int)(player_loc.y);
    printf("mapPosX: %d, mapPosY: %d\n", mapPosX, mapPosY);
    printf("player_loc.x: %f, player_loc.y: %f\n", player_loc.x, player_loc.y);

    if (map[mapPosX][mapPosY] > 0) {
      // map[mapPosX][mapPosY] = 4;
      printf("mapPosX: %d, mapPosY: %d\n", mapPosX, mapPosY);
      printf("hit\n");
    }

    for (int x = 0; x < w; x++) {
      double cameraX = 2 * x / (double)w - 1;
      double rayDirX = pd.x + cp.x * cameraX;
      double rayDirY = pd.y + cp.y * cameraX;

      Hit hit = walk_squares_to_find_hit(rayDirX, rayDirY);

      // Calculate height of line to draw on screen
      int lineHeight = (int)(h / hit.dist);

      // calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + h / 2;
      if (drawStart < 0)
        drawStart = 0;
      int drawEnd = lineHeight / 2 + h / 2;
      if (drawEnd >= h)
        drawEnd = h - 1;

      draw_vert_line(renderer, x, drawStart, drawEnd, hit.colour, hit.side);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  return 0;
}
