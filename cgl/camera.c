#include "camera.h"

#include <cglm/cglm.h>
#include <string.h>

#include "shader.h"

int init_camera(Camera *camera, ShaderProgram *shader_program) {
    memset(camera, 0, sizeof(Camera));
    camera->shader_program = shader_program;
    glm_perspective(75, 1280 / 1024, 0.1, 1000.0, camera->perspective);
    glm_translate_make((vec4 *)camera->view, (float *)camera->vec);
    return 0;
}

void update_camera(Camera *camera) {
    glm_translate_make((vec4 *)camera->view, (float *)camera->vec);
    set_uniform(get_uniform(camera->shader_program, "perspective"),
                (float *)camera->perspective);
    set_uniform(get_uniform(camera->shader_program, "view"),
                (float *)camera->view);
}