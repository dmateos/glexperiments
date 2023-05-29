#ifndef _MODEL_H
#define _MODEL_H

#include <cglm/cglm.h>

#include "shader.h"
#include "vertex.h"

#define OBJ_VERTEX_LIMIT 10240

typedef struct _Obj {
  float verticies[OBJ_VERTEX_LIMIT], normals[OBJ_VERTEX_LIMIT];
  unsigned int verticie_index[OBJ_VERTEX_LIMIT];
  unsigned int normal_index[OBJ_VERTEX_LIMIT];
  unsigned int ncount, vcount, vicount, nicount;
} ObjFile;

typedef struct _Model {
  float vec[3];
  mat4 translation;

  const ShaderProgram *program;
  VertexState state;
  VertexBuffer vertex, index, texture;
  ObjFile vdata;

  // Only used for instances
  unsigned int instance_count;
  VertexBuffer instance_buffer;
} Model;

int init_model(Model*, const ShaderProgram*, const char*, int, void*);
int draw_model(const Model* model);

#endif
