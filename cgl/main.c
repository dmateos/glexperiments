#include "main.h"

#include <stdbool.h>
#include <stdio.h>

#include "camera.h"
#include "model.h"
#include "shader.h"
#include "window.h"

int main(int argc, char **argv) {
    SDL_Event e;
    Window window;
    ShaderProgram shader_program;
    Model model;
    Camera camera;
    bool quit = false;

    init_window(&window, 1280, 1024);
    init_shaderprogram(&shader_program);
    add_shader(&shader_program, VERTEXSHADER, "shaders/vert.gsl");
    add_shader(&shader_program, FRAGSHADER, "shaders/frag.gsl");
    compile_shaderprogram(&shader_program);
    use_shaderprogram(&shader_program);

    init_camera(&camera, &shader_program);
    init_model(&model, &shader_program, argv[1]);

    camera.vec[Z] = -6;

    while (!quit) {
        while (poll_window(&window, &e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_a:
                            camera.vec[X] -= 0.1;
                            break;
                        case SDLK_d:
                            camera.vec[X] += 0.1;
                            break;
                        case SDLK_s:
                            camera.vec[Z] -= 0.1;
                            break;
                        case SDLK_w:
                            camera.vec[Z] += 0.1;
                            break;
                    }
                    break;
            }
        }

        clear_window();
        update_camera(&camera);
        draw_model(&model);
        swap_window(&window);
    }

    destroy_shaderprogram(&shader_program);
    destroy_window(&window);
    return 0;
}