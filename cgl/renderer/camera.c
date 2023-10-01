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
  glm_perspective(75, aspect, 0.1, 10000.0, camera->perspective);
}

int camera_init(Camera *camera, const ShaderProgram *shader_program,
                float aspect) {
  memset(camera, 0, sizeof(Camera));

  camera->position[0] = 0.0;
  camera->position[1] = 0.0;
  camera->position[2] = 10.0;

  camera->front[0] = 0.0;
  camera->front[1] = 0.0;
  camera->front[2] = -1.0;

  camera->yaw = -90;
  camera->pitch = 0;

  build_perspective(camera, aspect);
  camera_update(camera, (ShaderProgram *)shader_program, 0);
  return 0;
}

void camera_update(Camera *camera, ShaderProgram *shader,
                   int strip_translation) {
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

  shader_use(shader);
  shader_set_uniform(shader_get_uniform(shader, "perspective"),
                     (float *)camera->perspective);

  // For stuff like skyboxes we want to skip translation
  if (!strip_translation) {
    shader_set_uniform(shader_get_uniform(shader, "view"),
                       (float *)camera->view);
  } else {
    mat4 view;
    glm_mat4_copy(camera->view, view);
    view[3][0] = 0;
    view[3][1] = 0;
    view[3][2] = 0;

    shader_set_uniform(shader_get_uniform(shader, "view"), (float *)view);
  }
}

void camera_move(Camera *camera, CameraDirection direction) {
  const float velocity = 0.5;
  float tmp[3];

  if (direction == CAMERA_FORWARD) {
    glm_vec3_muladds(camera->front, velocity, camera->position);
  } else if (direction == CAMERA_BACK) {
    glm_vec3_scale(camera->front, velocity, tmp);
    glm_vec3_sub(camera->position, tmp, camera->position);
  } else if (direction == CAMERA_LEFT) {
    glm_vec3_scale(camera->right, velocity, tmp);
    glm_vec3_sub(camera->position, tmp, camera->position);
  } else if (direction == CAMERA_RIGHT) {
    glm_vec3_muladds(camera->right, velocity, camera->position);
  }
}

void camera_pivot(Camera *camera, int p, int y) {
  float yaw, pitch;
  yaw = p * 0.1;
  pitch = y * 0.1;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  camera->yaw += yaw;
  camera->pitch += pitch;
}