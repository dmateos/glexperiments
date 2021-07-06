#ifndef _CAMERA_H
#define _CAMERA_H

#include <cglm/cglm.h>

#include "shader.h"

typedef enum cameradirection {
    CAMERA_FORWARD,
    CAMERA_BACK,
    CAMERA_LEFT,
    CAMERA_RIGHT
} CameraDirection;

typedef struct Cam {
    mat4 view, perspective;
    const ShaderProgram *shader_program;
    float position[3], front[3], up[3], right[3];
    float yaw, pitch;
} Camera;

int init_camera(Camera *, const ShaderProgram *, float aspect);
void update_camera(Camera *);
void move_camera(Camera *, CameraDirection);
void pivot_camera(Camera *, int, int);

#endif