#ifndef _MODEL_H
#define _MODEL_H

#include <cglm/cglm.h>

#include "shader.h"
#include "texture.h"
#include "vertex.h"

typedef struct {
  float *verticies;
  unsigned int *verticie_index;
  unsigned int vcount, vicount;
} ObjFile;

typedef struct {
  float vec[3];
  mat4 translation;

  const ShaderProgram *program;
  VertexState state;
  VertexBuffer vertex, index;
  ObjFile vdata;
  Texture texture;

  // Only used for instances
  unsigned int instance_count;
  VertexBuffer instance_buffer;
} Model;

typedef struct {
  ShaderProgram program;
  VertexState state;
  VertexBuffer vertex;
  Texture texture;
} ModelSkybox;

void model_shader_init(ShaderProgram *shader, int instanced);
int model_init(Model *, const ShaderProgram *, const char *, const char *, int,
               void *);
int model_draw(const Model *model);
int model_proximity(Model *, float *, float);
void model_free(Model *model);

void model_skybox_init(ModelSkybox *skybox, const char *path);
int model_skybox_draw(const ModelSkybox *skybox);

#endif
