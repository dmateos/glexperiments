#ifndef _MODEL_H
#define _MODEL_H

#include <cglm/cglm.h>

#include "shader.h"
#include "vertex.h"

#define OBJ_VERTEX_LIMIT 102400

typedef struct {
  float verticies[OBJ_VERTEX_LIMIT];
  unsigned int verticie_index[OBJ_VERTEX_LIMIT];
  unsigned int vcount, vicount;
} ObjFile;

typedef struct {
  float vec[3];
  mat4 translation;

  const ShaderProgram *program;
  VertexState state;
  VertexBuffer vertex, index;
  ObjFile vdata;

  // Only used for instances
  unsigned int instance_count;
  VertexBuffer instance_buffer;
} Model;

int model_init(Model *, const ShaderProgram *, const char *, int, void *);
int model_draw(const Model *model);

#endif
