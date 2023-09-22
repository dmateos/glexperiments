#ifndef _SCENE_H_
#define _SCENE_H_

#include "camera.h"
#include "model.h"
#include "shader.h"

typedef struct {
  Camera camera;
  ShaderProgram shader;
  Model models[1024];
} Scene;

#endif