#ifndef _TEXTURE_H
#define _TEXTURE_H

typedef struct _textuer_t {
  unsigned int tbo, width, height;
} Texture;

int init_texture(Texture *, const char *);
int init_cubemap_texture(Texture *, const char *);
void bind_texture(const Texture *);
void bind_cubemap_texture(const Texture *);
void unbind_texture(const Texture *);
void unbind_cubemap_texture(const Texture *);

#endif