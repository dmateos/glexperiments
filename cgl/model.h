#ifndef _MODEL_H
#define _MODEL_H

#include "shader.h"
#include "vertex.h"

typedef struct _Model {
    float x, y, z;
    ShaderProgram *program;
    VertexState *state;

    float *verts, *normals;
    int *vert_index, *norm_index;
} Model;

int init_model(Model *, const char *);

#endif