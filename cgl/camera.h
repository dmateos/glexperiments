#ifndef _CAMERA_H
#define _CAMERA_H

#include <cglm/cglm.h>

#include "shader.h"

typedef enum cameradirection { UP, DOWN, LEFT, RIGHT } CameraDirection;
typedef struct Cam {
    mat4 view, perspective;
    const ShaderProgram *shader_program;
    float position[3], front[3], up[3], right[3], worldup[3];
    float yaw, pitch;
} Camera;

int init_camera(Camera *, const ShaderProgram *);
void move_camera(Camera *, CameraDirection);
void update_camera(Camera *);

#endif