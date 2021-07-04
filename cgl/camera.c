#include "camera.h"

#include <cglm/cglm.h>
#include <math.h>
#include <string.h>

#include "shader.h"

static float horizontal[3] = {1.0, 1.0, 1.0};

static void build_lookat(Camera *camera) {
    float front[3];
    glm_vec3_add(camera->position, camera->front, front);
    glm_lookat(camera->position, front, camera->up, camera->view);
}

static void build_perspective(Camera *camera) {
    glm_perspective(75, 1280 / 1024, 0.1, 1000.0, camera->perspective);
}

int init_camera(Camera *camera, const ShaderProgram *shader_program) {
    memset(camera, 0, sizeof(Camera));
    camera->shader_program = shader_program;

    camera->position[0] = 0.0;
    camera->position[1] = 0.0;
    camera->position[2] = 10.0;

    camera->up[0] = 0.0;
    camera->up[1] = 1.0;
    camera->up[2] = 0.0;

    camera->front[0] = 0.0;
    camera->front[1] = 0.0;
    camera->front[2] = -1.0;

    build_perspective(camera);
    update_camera(camera);
    return 0;
}

void update_camera(Camera *camera) {
    float front[3];
    front[0] = cos(camera->yaw * M_PI / 180) * cos(camera->pitch * M_PI / 180);
    front[1] = sin(camera->pitch * M_PI / 180);
    front[2] = cos(camera->yaw * M_PI / 180) * cos(camera->pitch * M_PI / 180);
    glm_normalize(front);

    build_lookat(camera);

    set_uniform(get_uniform(camera->shader_program, "perspective"),
                (float *)camera->perspective);
    set_uniform(get_uniform(camera->shader_program, "view"),
                (float *)camera->view);
}

void move_camera(Camera *camera, CameraDirection direction) {
    const float velocity = 5;

    if (direction == UP) {
        glm_vec3_muladds(camera->front, velocity, camera->position);
    } else if (direction == DOWN) {
        float tmp[3];
        glm_vec3_scale(camera->front, velocity, tmp);
        glm_vec3_sub(camera->position, tmp, camera->position);
    } else if (direction == LEFT) {
    } else if (direction == RIGHT) {
    }
}