#include "main.h"

#include <stdbool.h>
#include <stdio.h>

#include "model.h"
#include "shader.h"
#include "vertex.h"
#include "window.h"

const float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                          0.0f,  0.0f,  0.5f, 0.0f};
const unsigned int elements[] = {0, 1, 2};
float offsets[] = {-1.0f, 0.0f};

int main(int argc, char **argv) {
    SDL_Event e;
    Window window;
    ShaderProgram shader_program;
    VertexState state;
    VertexBuffer buffer, index_buffer;
    bool quit = false;
    Model model;

    init_window(&window, 1280, 1024);
    init_shaderprogram(&shader_program);

    init_model(&model, &shader_program, "models/cube.obj");

    add_shader(&shader_program, VERTEXSHADER, "shaders/vert.gsl");
    add_shader(&shader_program, FRAGSHADER, "shaders/frag.gsl");
    compile_shaderprogram(&shader_program);
    use_shaderprogram(&shader_program);

    init_vertex_state(&state, VERTEX_STATE_DRAW_INDEXED);
    bind_vertex_state(&state);

    init_vertex_buffer(&buffer, VERTEX_BUFFER_TYPE_ARRAY);
    bind_vertex_buffer(&buffer);
    write_vertex_buffer(&buffer, (void *)&vertices, sizeof(vertices));

    init_vertex_buffer(&index_buffer, VERTEX_BUFFER_TYPE_INDEX);
    bind_vertex_buffer(&index_buffer);
    write_vertex_buffer(&index_buffer, (void *)&elements, sizeof(elements));

    set_attribute(1, 3);

    while (!quit) {
        while (poll_window(&window, &e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_a:
                            offsets[0] -= 0.03;
                            break;
                        case SDLK_d:
                            offsets[0] += 0.03;
                            break;
                        case SDLK_s:
                            offsets[1] -= 0.03;
                            break;
                        case SDLK_w:
                            offsets[1] += 0.03;
                            break;
                    }
                    break;
            }
        }

        set_uniform(get_uniform(&shader_program, "offset"), offsets);

        clear_window();
        // draw(&model.state, model.vdata.vcount);
        draw(&state, sizeof(elements) / sizeof(unsigned int));
        swap_window(&window);
    }

    destroy_shaderprogram(&shader_program);
    destroy_window(&window);
    return 0;
}