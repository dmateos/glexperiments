#include "main.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define VERT 600
#define HORIZ 800
#define MAPX 8
#define MAPY 8
#define SCREEN_OFFSETX 150
#define RECTSIZE 16
#define TSPEED 0.10
#define w 800
#define h 480

typedef struct {
  double x, y;
} Vec2;

typedef struct {
  int colour, side;
  double dist;
} Hit;

typedef struct {
  Vec2 loc, dir, cam;
} Player;

Player player = {
    .loc = {1, 2},     //
    .dir = {1.0, 0.0}, //
    .cam = {0.0, 0.66} //
};

int map[MAPX][MAPY] = {
    {1, 1, 1, 1, 1, 1, 1, 1}, //
    {1, 0, 0, 0, 0, 0, 0, 1}, //
    {1, 3, 3, 0, 0, 0, 0, 1}, //
    {1, 3, 3, 0, 0, 0, 0, 1}, //
    {1, 0, 0, 0, 2, 2, 0, 1}, //
    {1, 0, 0, 0, 2, 2, 0, 1}, //
    {1, 0, 0, 0, 0, 0, 0, 1}, //
    {1, 1, 1, 1, 1, 1, 1, 1}, //
};

int screenbuffer[w][h];

void draw_screenbuffer(SDL_Renderer *renderer) {
  for (int i = 0; i < w; i++) {
    for (int y = 0; y < h; i++) {
      SDL_RenderDrawPoint(renderer, i, y);
    }
  }
}

void draw_map(SDL_Renderer *renderer) {
  for (int i = 0; i < MAPX; i++) {
    for (int j = 0; j < MAPY; j++) {
      if (map[i][j] > 0) {
        switch (map[i][j]) {
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
        SDL_Rect rect = {i * RECTSIZE, j * RECTSIZE, RECTSIZE, RECTSIZE};
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }

  // draw player
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_Rect rect = {player.loc.x * RECTSIZE, player.loc.y * RECTSIZE, 4, 4};
  SDL_RenderFillRect(renderer, &rect);

  // draw camera direction
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawLine(renderer, player.loc.x * RECTSIZE, player.loc.y * RECTSIZE,
                     player.loc.x * RECTSIZE + player.dir.x * 100,
                     player.loc.y * RECTSIZE + player.dir.y * 100);
}

void draw_vert_line(SDL_Renderer *r, int x, int start, int end, int c, int s) {
  int divider = s == 0 ? 1 : 2;

  switch (c) {
  case 1:
    SDL_SetRenderDrawColor(r, 255 / divider, 0, 0, 255);
    break;
  case 2:
    SDL_SetRenderDrawColor(r, 0, 255 / divider, 0, 255);
    break;
  case 3:
    SDL_SetRenderDrawColor(r, 0, 0, 255 / divider, 255);
    break;
  }

  SDL_RenderDrawLine(r, x, start + SCREEN_OFFSETX, x, end + SCREEN_OFFSETX);
}

Hit walk_squares_to_find_hit(double rayDirX, double rayDirY) {
  int mapPosX = (int)(player.loc.x);
  int mapPosY = (int)(player.loc.y);
  int hit = 0;
  int stepX, stepY, side;
  double distX, distY = 0;
  double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
  double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
  double perpWallDist;

  if (rayDirX < 0) {
    stepX = -1;
    distX = (player.loc.x - mapPosX) * deltaDistX;
  } else {
    stepX = 1;
    distX = (mapPosX + 1.0 - player.loc.x) * deltaDistX;
  }

  if (rayDirY < 0) {
    stepY = -1;
    distY = (player.loc.y - mapPosY) * deltaDistY;
  } else {
    stepY = 1;
    distY = (mapPosY + 1.0 - player.loc.y) * deltaDistY;
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

  window = SDL_CreateWindow("Test", 0, 0, HORIZ, VERT, SDL_WINDOW_RESIZABLE);
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
          new_x = player.loc.x + player.dir.x * 0.1;
          new_y = player.loc.y + player.dir.y * 0.1;

          if (map[(int)new_x][(int)player.loc.y] == 0) {
            player.loc.x = player.loc.x + player.dir.x * 0.1;
          }
          if (map[(int)player.loc.x][(int)new_y] == 0) {
            player.loc.y = player.loc.y + player.dir.y * 0.1;
          }
          break;
        case SDLK_s:
          new_x = player.loc.x + player.dir.x * 0.1;
          new_y = player.loc.y + player.dir.y * 0.1;

          if (map[(int)new_x][(int)player.loc.y] == 0) {
            player.loc.x = player.loc.x + player.dir.x * -0.1;
          }
          if (map[(int)player.loc.x][(int)new_y] == 0) {
            player.loc.y = player.loc.y + player.dir.y * -0.1;
          }
          break;
        case SDLK_a:
          old_x = player.dir.x;
          player.dir.x = old_x * cos(TSPEED) + player.dir.y * sin(TSPEED);
          player.dir.y = -old_x * sin(TSPEED) + player.dir.y * cos(TSPEED);
          old_x = player.cam.x;
          player.cam.x = old_x * cos(TSPEED) + player.cam.y * sin(TSPEED);
          player.cam.y = -old_x * sin(TSPEED) + player.cam.y * cos(TSPEED);
          break;
        case SDLK_d:
          // turn right
          old_x = player.dir.x;
          player.dir.x = old_x * cos(-TSPEED) + player.dir.y * sin(-TSPEED);
          player.dir.y = -old_x * sin(-TSPEED) + player.dir.y * cos(-TSPEED);
          old_x = player.cam.x;
          player.cam.x = old_x * cos(-TSPEED) + player.cam.y * sin(-TSPEED);
          player.cam.y = -old_x * sin(-TSPEED) + player.cam.y * cos(-TSPEED);
          break;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    draw_map(renderer);

    // convert player pos to map pos
    int mapPosX = (int)(player.loc.x);
    int mapPosY = (int)(player.loc.y);
    printf("mapPosX: %d, mapPosY: %d\n", mapPosX, mapPosY);
    printf("player.loc.x: %f, player.loc.y: %f\n", player.loc.x, player.loc.y);

    if (map[mapPosX][mapPosY] > 0) {
      // map[mapPosX][mapPosY] = 4;
      printf("mapPosX: %d, mapPosY: %d\n", mapPosX, mapPosY);
      printf("hit\n");
    }

    for (int x = 0; x < w; x++) {
      // adjust coordinates of camera
      // so that it goes from -1 to 1 with 0 in the middle
      double cameraX = 2 * x / (double)w - 1;
      double rayDirX = player.dir.x + player.cam.x * cameraX;
      double rayDirY = player.dir.y + player.cam.y * cameraX;

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
