#ifndef _TEXTURE_H
#define _TEXTURE_H

typedef struct _textuer_t {
  unsigned int tbo, width, height;
} Texture;

int texture_init(Texture *, const char *);
int texture_init_cubemap(Texture *, const char *);
void texture_bind(const Texture *);
void texture_bind_cubemap(const Texture *);
void texture_unbind(const Texture *);
void texture_unbind_cubemap(const Texture *);

#endif