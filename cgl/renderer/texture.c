#include "texture.h"

#include <OpenGL/gl3.h>
#include <SDL2/SDL_image.h>
#include <string.h>

static SDL_Surface *load_image(const char *path) {
  SDL_Surface *image = IMG_Load(path);

  if (image != NULL) {
    printf("loaded image %s from disk\n", path);
  } else {
    printf("could not load image\n");
    exit(1);
  }

  // check if image dimensions are powers of 2
  if ((image->w & (image->w - 1)) != 0) {
    printf("image %s's width is not a power of 2\n", path);
    exit(1);
  }

  return image;
}

int texture_init(Texture *t, const char *path) {
  memset(t, 0, sizeof(Texture));
  SDL_Surface *img = load_image(path);
  int mode = GL_RGB;

  glGenTextures(1, &t->tbo);
  texture_bind(t);

  if (img->format->BytesPerPixel == 4) {
    mode = GL_RGBA;
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, mode, img->w, img->h, 0, mode,
               GL_UNSIGNED_BYTE, img->pixels);

  glGenerateMipmap(GL_TEXTURE_2D);
  printf("inited new texture %u with %dx%d dimentions\n", t->tbo, img->w,
         img->h);
  SDL_FreeSurface(img);
  return 0;
}

int texture_init_cubemap(Texture *t, const char *path) {
  memset(t, 0, sizeof(Texture));
  int mode = GL_RGB;
  const char *paths[] = {"right.jpg",  "left.jpg",  "top.jpg",
                         "bottom.jpg", "front.jpg", "back.jpg"};

  glGenTextures(1, &t->tbo);
  texture_bind_cubemap(t);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  for (int i = 0; i < 6; i++) {
    char path_combined[256];
    memset(path_combined, 0, 256);
    strncat(path_combined, path, 256);
    strncat(path_combined, paths[i], 256);
    SDL_Surface *img = load_image(path_combined);
    if (img->format->BytesPerPixel == 4) {
      mode = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mode, img->w, img->h, 0,
                 mode, GL_UNSIGNED_BYTE, img->pixels);
    printf("inited new skymap texture %u (%d) with %dx%d dimentions\n", t->tbo,
           i, img->w, img->h);
    SDL_FreeSurface(img);
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  return 0;
}

void texture_bind(const Texture *t) { glBindTexture(GL_TEXTURE_2D, t->tbo); }

void texture_bind_cubemap(const Texture *t) {
  glBindTexture(GL_TEXTURE_CUBE_MAP, t->tbo);
}

void texture_unbind(const Texture *t) { glBindTexture(GL_TEXTURE_2D, 0); }

void texture_unbind_cubemap(const Texture *t) {
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}