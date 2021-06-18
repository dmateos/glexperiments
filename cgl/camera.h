#ifndef _CAMERA_H
#define _CAMERA_H

#include <cglm/cglm.h>

#include "shader.h"

typedef struct Cam {
    mat4 view, perspective;
    const ShaderProgram *shader_program;
    float position[3], front[3], up[3], target[3];
} Camera;

int init_camera(Camera *, const ShaderProgram *);
void update_camera(Camera *);
void move_camera_forward(Camera *);
void move_camera_backward(Camera *);
void strafe_camera_left(Camera *camera);
void strafe_camera_right(Camera *camera);

#endif