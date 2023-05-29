#include <SDL2/SDL.h>
#include <stdio.h>

#define X 256
#define Y 256
#define PX 4
typedef enum { EMPTY, SAND, WATER, FIRE, PLANT } element_types;

uint8_t world[X][Y];
uint8_t current_element = SAND;

#define BELOW(w, x, y) (w[x][y + 1])
#define ABOVE(w, x, y) (w[x][y - 1])
#define LEFT(w, x, y) (w[x - 1][y])
#define RIGHT(w, x, y) (w[x + 1][y])
#define REPLACE(x, y, v) (world[x][y] = v)

void handle_mouse_press(SDL_MouseButtonEvent e) {
  int x, y;
  SDL_GetMouseState(&x, &y);
  printf("x: %d, y: %d\n", x, y);

  if (e.button == SDL_BUTTON_RIGHT) {
    REPLACE(x / PX, y / PX, current_element);
  } else if (e.button == SDL_BUTTON_LEFT) {
    REPLACE(x / PX, y / PX, EMPTY);
  }
}

inline char bounds_check(int x, int y) {
  return x >= 0 && x < X && y >= 0 && y < Y;
}

void draw_world(SDL_Renderer *renderer) {
  for (int x = 0; x < X; x++) {
    for (int y = 0; y < Y; y++) {
      SDL_Rect rect = {x * PX, y * PX, PX, PX};

      switch (world[x][y]) {
      case EMPTY:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        break;
      case SAND:
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        break;
      case WATER:
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        break;
      case FIRE:
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        break;
      case PLANT:
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        break;
      }
    }
  }
}

void compute_world(void) {
  uint8_t world_copy[X][Y];
  int direction;
  memcpy(world_copy, world, sizeof(world));

  for (int x = 0; x < X; x++) {
    for (int y = 0; y < Y; y++) {
      switch (world_copy[x][y]) {
      case EMPTY:
        break;
      case SAND:
        if (BELOW(world_copy, x, y) == EMPTY) {
          if (y + 1 < Y) {
            REPLACE(x, y, EMPTY);
            REPLACE(x, y + 1, SAND);
          }
        }
        break;
      case WATER:
        break;
      case FIRE:
        direction = rand() % 4;
        REPLACE(x, y, EMPTY);
        if (direction == 1 && ABOVE(world_copy, x, y) == PLANT) {
          if (bounds_check(x, y - 1)) {
            REPLACE(x, y - 1, FIRE);
          }
        } else if (direction == 2 && LEFT(world_copy, x, y) == PLANT) {
          if (bounds_check(x - 1, y)) {
            REPLACE(x - 1, y, FIRE);
          }
        } else if (direction == 3 && RIGHT(world_copy, x, y) == PLANT) {
          if (bounds_check(x + 1, y)) {
            REPLACE(x + 1, y, FIRE);
          }
        } else if (BELOW(world_copy, x, y) == PLANT) {
          if (bounds_check(x, y + 1)) {
            REPLACE(x, y + 1, FIRE);
          }
        }
        break;
      case PLANT:
        direction = rand() % 4;
        if (direction == 1 && ABOVE(world_copy, x, y) == EMPTY) {
          if (bounds_check(x, y - 1)) {
            REPLACE(x, y - 1, PLANT);
          }
        } else if (direction == 2 && LEFT(world_copy, x, y) == EMPTY) {
          if (bounds_check(x - 1, y)) {
            REPLACE(x - 1, y, PLANT);
          }
        } else if (direction == 3 && RIGHT(world_copy, x, y) == EMPTY) {
          if (bounds_check(x + 1, y)) {
            REPLACE(x + 1, y, PLANT);
          }
        }
        break;
      }
    }
  }
}

int main(void) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event e;

  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    printf("could not init window subsystem\n");
    exit(1);
  }

  for (int x = 0; x < X; x++) {
    for (int y = 0; y < Y; y++) {
      world[x][y] = rand() % 2;
    }
  }

  window = SDL_CreateWindow("Test", 0, 0, X * PX, Y * PX, SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  while (1) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        exit(0);
      case SDL_MOUSEBUTTONDOWN:
        handle_mouse_press(e.button);
        break;
      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
        case SDLK_1:
          current_element = SAND;
          break;
        case SDLK_2:
          current_element = WATER;
          break;
        case SDLK_3:
          current_element = FIRE;
          break;
        case SDLK_4:
          current_element = PLANT;
          break;
        }
        break;
      }
    }

    compute_world();
    draw_world(renderer);
    SDL_RenderPresent(renderer);
    // SDL_Delay(1);
  }

  return 0;
}