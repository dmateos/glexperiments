#ifndef _MODEL_H
#define _MODEL_H

#include "shader.h"

typedef struct _Model {
    float x, y, z;
    ShaderProgram *program;

    float *verts, *normals;
    int *vert_index, *norm_index;
} Model;

int init_model(Model *, const char *);

#endif