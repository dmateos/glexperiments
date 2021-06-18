#include "main.h"

#include <stdbool.h>
#include <stdio.h>

#include "camera.h"
#include "model.h"
#include "shader.h"
#include "window.h"

const float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                          0.0f,  0.0f,  0.5f, 0.0f};
const unsigned int elements[] = {0, 1, 2};

int main(int argc, char **argv) {
    SDL_Event e;
    Window window;
    ShaderProgram shader_program;
    Model model, model2;
    Camera camera;
    bool quit = false;

    init_window(&window, 1280, 1024);
    init_shaderprogram(&shader_program);

    add_shader(&shader_program, VERTEXSHADER, "shaders/vert.gsl");
    add_shader(&shader_program, FRAGSHADER, "shaders/frag.gsl");
    compile_shaderprogram(&shader_program);
    use_shaderprogram(&shader_program);

    init_model(&model, &shader_program, "models/cube.obj");
    init_model(&model2, &shader_program, "models/cube.obj");

    while (!quit) {
        while (poll_window(&window, &e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_a:
                            model.vec[X] -= 0.1;
                            break;
                        case SDLK_d:
                            model.vec[X] += 0.1;
                            break;
                        case SDLK_s:
                            model.vec[Y] -= 0.1;
                            break;
                        case SDLK_w:
                            model.vec[Y] += 0.1;
                            break;
                    }
                    break;
            }
        }

        clear_window();
        draw_model(&model);
        draw_model(&model2);
        swap_window(&window);
    }

    destroy_shaderprogram(&shader_program);
    destroy_window(&window);
    return 0;
}