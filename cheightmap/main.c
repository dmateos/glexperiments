#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "main.h"

typedef struct {
  int width, height, passes;
  png_byte color_type, bit_depth;
  png_structp png_ptr;
  png_infop info_ptr;
  png_bytep *row_ptrs;
} Image;

static float rgb2lum(float r, float g, float b) {
  float v = (0.2126 * r + 0.7152 * g + 0.0722 * b);
  return v;
}

static void read_png(const char *filename, Image *img) {
  char header[8];
  FILE *fp = fopen(filename, "rb");

  fread(header, 1, 8, fp);

  if (png_sig_cmp((png_const_bytep)header, 0, 8)) {
    printf("png header error\n");
    exit(1);
  }

  img->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!img->png_ptr) {
    printf("png_ptr error\n");
    exit(1);
  }
  img->info_ptr = png_create_info_struct(img->png_ptr);
  if (!img->info_ptr) {
    printf("info_ptr error\n");
    exit(1);
  }

  if (setjmp(png_jmpbuf(img->png_ptr))) { // wtf
    printf("setjmp error\n");
    exit(1);
  }

  png_init_io(img->png_ptr, fp);
  png_set_sig_bytes(img->png_ptr, 8);
  png_read_info(img->png_ptr, img->info_ptr);

  img->width = png_get_image_width(img->png_ptr, img->info_ptr);
  img->height = png_get_image_height(img->png_ptr, img->info_ptr);
  img->color_type = png_get_color_type(img->png_ptr, img->info_ptr);
  img->bit_depth = png_get_bit_depth(img->png_ptr, img->info_ptr);
  img->passes = png_set_interlace_handling(img->png_ptr);
  png_read_update_info(img->png_ptr, img->info_ptr);

  if (setjmp(png_jmpbuf(img->png_ptr))) { // wtf
    printf("setjmp error\n");
    exit(1);
  }

  img->row_ptrs = malloc(sizeof(png_bytep) * img->height);
  for (int y = 0; y < img->height; y++) {
    img->row_ptrs[y] = malloc(png_get_rowbytes(img->png_ptr, img->info_ptr));
  }
  png_read_image(img->png_ptr, img->row_ptrs);

  printf("%d %d %d %x %x\n", img->width, img->height, img->passes, img->color_type, img->bit_depth);
  fclose(fp);
}

void free_png(Image *img) {
  for (int y = 0; y < img->height; y++) {
    free(img->row_ptrs[y]);
  }
  free(img->row_ptrs);
  // png_destroy_info_struct(&img->png_ptr, &img->info_ptr);
  //  png_destroy_read_struct(&img->png_ptr, NULL, NULL);
}

void print_png(Image *img, SDL_Renderer *renderer) {
  for (int y = 0; y < img->height; y++) {
    png_byte *row = img->row_ptrs[y];
    for (int x = 0; x < img->width; x++) {
      png_byte *ptr = &(row[x * 1]); // Need to tweak this offset
      // printf("%d - %d ]: %d - %d - %d - %d\n", x, y, ptr[0], ptr[1], ptr[2], ptr[3]);
      // float lum = rgb2lum(ptr[0], ptr[1], ptr[2]);
      SDL_SetRenderDrawColor(renderer, ptr[0], ptr[1], ptr[2], ptr[3]);
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
}

int main(int argc, char **argv) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event e;
  Image test_img;
  int cont = 1;

  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    printf("could not init window subsystem\n");
    exit(1);
  }

  if (argc < 2) {
    printf("specify img\n");
    exit(1);
  }
  read_png(argv[1], &test_img);

  window = SDL_CreateWindow("Test", 0, 0, test_img.width, test_img.height, SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, -1, 0);

  while (cont) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        cont = 0;
      }
    }

    SDL_RenderClear(renderer);
    print_png(&test_img, renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(100);
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  free_png(&test_img);
  return 0;
}