#include "texture.h"

#include <OpenGL/gl3.h>
#include <SDL2/SDL_image.h>
#include <string.h>

static SDL_Surface *load_image(const char *path) {
    SDL_Surface *image;
    SDL_RWops *rwop;

    rwop = SDL_RWFromFile(path, "rb");
    image = IMG_LoadJPG_RW(rwop);

    if (image != NULL) {
        printf("loaded image %s from disk\n", path);
    }

    SDL_RWclose(rwop);
    return image;
}

int init_texture(Texture *t, const char *path) {
    memset(t, 0, sizeof(Texture));
    SDL_Surface *img = load_image(path);

    glGenTextures(1, &t->tbo);
    bind_texture(t);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, img->pixels);

    SDL_FreeSurface(img);
    return 0;
}

void bind_texture(Texture *t) { glBindTexture(GL_TEXTURE_2D, t->tbo); }