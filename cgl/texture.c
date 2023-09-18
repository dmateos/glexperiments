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

  return image;
}

int init_texture(Texture *t, const char *path) {
  memset(t, 0, sizeof(Texture));
  SDL_Surface *img = load_image(path);
  int mode = GL_RGB;

  glGenTextures(1, &t->tbo);
  bind_texture(t);
  printf("inited new texture %u with %d and %d dimentions\n", t->tbo, img->w,
         img->h);

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
  SDL_FreeSurface(img);
  return 0;
}

int init_cubemap_texture(Texture *t, const char *path) {
  memset(t, 0, sizeof(Texture));
  SDL_Surface *img = load_image(path);
  int mode = GL_RGB;

  glGenTextures(1, &t->tbo);
  bind_cubemap_texture(t);
  if (img->format->BytesPerPixel == 4) {
    mode = GL_RGBA;
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  for (int i = 0; i < 6; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mode, img->w, img->h, 0,
                 mode, GL_UNSIGNED_BYTE, img->pixels);
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  SDL_FreeSurface(img);
  return 0;
}

void bind_texture(const Texture *t) { glBindTexture(GL_TEXTURE_2D, t->tbo); }

void bind_cubemap_texture(const Texture *t) {
  glBindTexture(GL_TEXTURE_CUBE_MAP, t->tbo);
}

void unbind_texture(const Texture *t) { glBindTexture(GL_TEXTURE_2D, 0); }

void unbind_cubemap_texture(const Texture *t) {
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}