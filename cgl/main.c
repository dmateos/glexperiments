#include "main.h"

#include <stdbool.h>
#include <stdio.h>

#include "shader.h"
#include "window.h"

int main() {
    Window *window = init_window(1280, 1024);

    ShaderProgram *shader_program = init_shaderprogram();
    use_shaderprogram(shader_program);
    add_shader(shader_program, VERTEXSHADER, "render/vert.gsl");
    add_shader(shader_program, FRAGSHADER, "render/frag.gsl");

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (poll_window(window, &e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        swap_window(window);
    }

    destroy_window(window);
    return 0;
}