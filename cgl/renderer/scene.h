#ifndef _SCENE_H_
#define _SCENE_H_

#include "camera.h"
#include "model.h"
#include "shader.h"
#include "texture.h"

typedef struct _sn {
  struct _sn *next;
  Model models[1024];
} SceneNode;

typedef struct {
  Camera camera;
  ShaderProgram shader;
  SceneNode *head;
  ModelSkybox skybox;

  Model test_model;
} Scene;

void scene_init(Scene *scene);
void scene_draw(Scene *scene);
void scene_skybox_init(Scene *scene);
void scene_free(Scene *scene);

#endif