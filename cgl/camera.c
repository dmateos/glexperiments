#include "camera.h"

#include <cglm/cglm.h>
#include <string.h>

#include "shader.h"

static void build_lookat(Camera *camera) {
    glm_lookat(camera->position, camera->target, camera->up, camera->view);
}

static void build_perspective(Camera *camera) {
    glm_perspective(75, 1280 / 1024, 0.1, 1000.0, camera->perspective);
}

int init_camera(Camera *camera, const ShaderProgram *shader_program) {
    memset(camera, 0, sizeof(Camera));
    camera->shader_program = shader_program;

    camera->position[0] = 0.0;
    camera->position[1] = 0.0;
    camera->position[2] = -10.0;

    camera->front[0] = 0.0;
    camera->front[1] = 0.0;
    camera->front[2] = 1.0;

    camera->up[0] = 0.0;
    camera->up[1] = 1.0;
    camera->up[2] = 0.0;

    camera->target[0] = camera->position[0] + camera->front[0];
    camera->target[1] = camera->position[1] + camera->front[1];
    camera->target[2] = camera->position[2] + camera->front[2];

    build_perspective(camera);
    build_lookat(camera);
    return 0;
}

void move_camera_forward(Camera *camera) {
    float posandfront[3];
    float horizontal[3] = {1.0, 1.0, 1.0};
    glm_vec3_add(camera->position, camera->front, posandfront);
    glm_vec3_mul(posandfront, horizontal, camera->position);
    build_lookat(camera);
}

void move_camera_backward(Camera *camera) {
    float posandfront[3];
    float horizontal[3] = {1.0, 1.0, 1.0};
    glm_vec3_sub(camera->position, camera->front, posandfront);
    glm_vec3_mul(posandfront, horizontal, camera->position);
    build_lookat(camera);
}

void strafe_camera_left(Camera *camera) { return 0; }

void strafe_camera_right(Camera *camera) { return 0; }

void update_camera(Camera *camera) {
    set_uniform(get_uniform(camera->shader_program, "perspective"),
                (float *)camera->perspective);
    set_uniform(get_uniform(camera->shader_program, "view"),
                (float *)camera->view);
}