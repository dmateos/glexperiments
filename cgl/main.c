#include "main.h"

#include <stdbool.h>
#include <stdio.h>

#include "shader.h"
#include "vertex.h"
#include "window.h"

int main() {
    Window window;
    ShaderProgram shader_program;
    SDL_Event e;
    bool quit = false;

    VertexState state;
    VertexBuffer buffer;

    init_window(&window, 1280, 1024);
    init_shaderprogram(&shader_program);
    init_vertex_state(&state);
    init_vertex_buffer(&buffer);

    use_shaderprogram(&shader_program);
    add_shader(&shader_program, VERTEXSHADER, "shaders/vert.gsl");
    add_shader(&shader_program, FRAGSHADER, "shaders/frag.gsl");
    compile_shaderprogram(&shader_program);

    while (!quit) {
        while (poll_window(&window, &e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        bind_vertex_state(&state);
        draw_array(&state);
        swap_window(&window);
    }

    destroy_shaderprogram(&shader_program);
    destroy_window(&window);
    return 0;
}