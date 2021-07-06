#ifndef _TEXTURE_H
#define _TEXTURE_H

typedef struct _textuer_t {
    unsigned int tbo, width, height;
} Texture;

int init_texture(Texture*, const char*);
void bind_texture(Texture*);

#endif