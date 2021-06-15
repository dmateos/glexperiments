#ifndef _MODEL_H
#define _MODEL_H

#include "shader.h"
#include "vertex.h"

#define OBJ_VERTEX_LIMIT 1024

typedef struct _Model {
    float x, y, z;
    ShaderProgram *program;
    VertexState *state;

    float *verts, *normals;
    int *vert_index, *norm_index;
} Model;

typedef struct _Obj {
    float verticies[OBJ_VERTEX_LIMIT], normals[OBJ_VERTEX_LIMIT];
    unsigned int verticie_index[OBJ_VERTEX_LIMIT];
    unsigned int normal_index[OBJ_VERTEX_LIMIT];
    unsigned int ncount, vcount, vicount, nicount;
} ObjFile;

int init_model(Model *, const char *);

#endif