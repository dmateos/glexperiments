#include "main.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "camera.h"
#include "model.h"
#include "shader.h"
#include "vertex.h"
#include "window.h"

#define WINDOW_HORIZ 2300
#define WINDOW_VERT 1600
#define MODEL_COUNT 1024

int main(int argc, char **argv) {
    SDL_Event e;
    Window window;
    ShaderProgram shader_program;
    Model *model;
    Camera camera;
    bool quit = false;

    if (argc < 2) {
        printf("specify models to load\n");
        exit(1);
    }

    init_window(&window, WINDOW_HORIZ, WINDOW_VERT);
    init_shaderprogram(&shader_program);
    add_shader(&shader_program, VERTEXSHADER, "shaders/vert.gsl");
    add_shader(&shader_program, FRAGSHADER, "shaders/frag.gsl");
    compile_shaderprogram(&shader_program);
    use_shaderprogram(&shader_program);

    init_camera(&camera, &shader_program, WINDOW_HORIZ / WINDOW_VERT);

    model = malloc(sizeof(Model) * MODEL_COUNT);

    int col = 0;
    int row = 0;
    int rcount = 0;
    for (int i = 0; i < MODEL_COUNT; i++) {
        init_model(&model[i], &shader_program, argv[1]);
        model[i].vec[2] += row;
        model[i].vec[0] += col;

        row += 3;
        rcount += 1;

        if ((rcount % (int)sqrt(MODEL_COUNT) == 0) && (rcount != 0)) {
            row = 0;
            col += 3;
            rcount = 0;
        }
    }

    while (!quit) {
        while (poll_window(&window, &e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_a:
                            move_camera(&camera, CAMERA_RIGHT);
                            break;
                        case SDLK_d:
                            move_camera(&camera, CAMERA_LEFT);
                            break;
                        case SDLK_s:
                            move_camera(&camera, CAMERA_BACK);
                            break;
                        case SDLK_w:
                            move_camera(&camera, CAMERA_FORWARD);
                            break;
                        case SDLK_RIGHT:
                            pivot_camera(&camera, 4, 0);
                            break;
                        case SDLK_LEFT:
                            pivot_camera(&camera, -4, 0);
                            break;
                        case SDLK_UP:
                            pivot_camera(&camera, 0, 4);
                            break;
                        case SDLK_DOWN:
                            pivot_camera(&camera, 0, -4);
                            break;
                    }
                    break;
            }
        }

        // SDL_GetMouseState(&mousex, &mousey);
        // pivot_camera(&camera, mousex * 0.01, 0);

        clear_window();
        update_camera(&camera);
        for (int i = 0; i < MODEL_COUNT; i++) {
            draw_model(&model[i]);
        }
        swap_window(&window);
    }

    destroy_shaderprogram(&shader_program);
    destroy_window(&window);
    free(model);
    return 0;
}