#include "main.h"

#include <stdbool.h>
#include <stdio.h>

#include "shader.h"
#include "window.h"

int main() {
    Window window;
    ShaderProgram shader_program;
    SDL_Event e;
    bool quit = false;

    init_window(&window, 1280, 1024);
    init_shaderprogram(&shader_program);

    use_shaderprogram(&shader_program);
    add_shader(&shader_program, VERTEXSHADER, "shaders/vert.gsl");
    add_shader(&shader_program, FRAGSHADER, "shaders/frag.gsl");

    while (!quit) {
        while (poll_window(&window, &e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        swap_window(&window);
    }

    destroy_window(&window);
    return 0;
}