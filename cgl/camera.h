#ifndef _CAMERA_H
#define _CAMERA_H

#include <cglm/cglm.h>

typedef struct Cam {
    float vec[3];
    mat4 view, perspective;
} Camera;

int init_camera(Camera*);
#endif