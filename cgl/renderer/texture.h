#ifndef _TEXTURE_H
#define _TEXTURE_H

typedef enum {
  TEXTURE_2D,
  TEXTURE_CUBEMAP,
} TextureType;

typedef struct {
  unsigned int tbo, width, height;
  TextureType type;
} Texture;

int texture_init(Texture *, const char *);
int texture_init_cubemap(Texture *, const char *);
void texture_bind(const Texture *);
void texture_unbind(const Texture *);

#endif