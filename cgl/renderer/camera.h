#ifndef _CAMERA_H
#define _CAMERA_H

#include <cglm/cglm.h>

#include "shader.h"

typedef enum {
  CAMERA_FORWARD,
  CAMERA_BACK,
  CAMERA_LEFT,
  CAMERA_RIGHT
} CameraDirection;

typedef struct {
  mat4 view, perspective;
  const ShaderProgram *shader_program;
  float position[3], front[3], up[3], right[3];
  float yaw, pitch;
} Camera;

int camera_init(Camera *, const ShaderProgram *, float aspect);
void camera_update(Camera *);
void camera_move(Camera *, CameraDirection);
void camera_pivot(Camera *, int, int);
void camerea_update_uniforms(Camera *camera, ShaderProgram *shader);

#endif