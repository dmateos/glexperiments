#ifndef _CAMERA_H
#define _CAMERA_H

#include <cglm/cglm.h>

#include "shader.h"

typedef struct Cam {
    float vec[3];
    mat4 view, perspective;
    const ShaderProgram *shader_program;
} Camera;

int init_camera(Camera *, const ShaderProgram *);
void update_camera(Camera *);
#endif