#include "camera.h"

#include <cglm/cglm.h>
#include <math.h>
#include <string.h>

#include "shader.h"

static float worldup[3] = {0.0, -1.0, 0.0};

static void build_lookat(Camera *camera) {
    float front[3];
    glm_vec3_add(camera->position, camera->front, front);
    glm_lookat(camera->position, front, camera->up, camera->view);
}

static void build_perspective(Camera *camera, float aspect) {
    glm_perspective(75, aspect, 0.1, 1000.0, camera->perspective);
}

int init_camera(Camera *camera, const ShaderProgram *shader_program,
                float aspect) {
    memset(camera, 0, sizeof(Camera));
    camera->shader_program = shader_program;

    camera->position[0] = 0.0;
    camera->position[1] = 0.0;
    camera->position[2] = 10.0;

    camera->front[0] = 0.0;
    camera->front[1] = 0.0;
    camera->front[2] = -1.0;

    camera->yaw = -90;
    camera->pitch = 0;

    build_perspective(camera, aspect);
    update_camera(camera);
    return 0;
}

void update_camera(Camera *camera) {
    camera->front[0] =
        cos(camera->yaw * M_PI / 180) * cos(camera->pitch * M_PI / 180);
    camera->front[1] = sin(camera->pitch * M_PI / 180);
    camera->front[2] =
        sin(camera->yaw * M_PI / 180) * cos(camera->pitch * M_PI / 180);
    glm_normalize(camera->front);

    glm_cross(camera->front, worldup, camera->right);
    glm_normalize(camera->right);
    glm_cross(camera->right, camera->front, camera->up);
    glm_normalize(camera->up);

    build_lookat(camera);

    set_uniform(get_uniform(camera->shader_program, "perspective"),
                (float *)camera->perspective);
    set_uniform(get_uniform(camera->shader_program, "view"),
                (float *)camera->view);
}

void move_camera(Camera *camera, CameraDirection direction) {
    const float velocity = 0.5;

    if (direction == CAMERA_FORWARD) {
        glm_vec3_muladds(camera->front, velocity, camera->position);
    } else if (direction == CAMERA_BACK) {
        float tmp[3];
        glm_vec3_scale(camera->front, velocity, tmp);
        glm_vec3_sub(camera->position, tmp, camera->position);
    } else if (direction == CAMERA_LEFT) {
        float tmp[3];
        glm_vec3_scale(camera->right, velocity, tmp);
        glm_vec3_sub(camera->position, tmp, camera->position);
    } else if (direction == CAMERA_RIGHT) {
        glm_vec3_muladds(camera->right, velocity, camera->position);
    }
}

void pivot_camera(Camera *camera, int p, int y) {
    float yaw, pitch;
    yaw = p * 0.1;
    pitch = y * 0.1;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    camera->yaw += yaw;
    camera->pitch += pitch;
}