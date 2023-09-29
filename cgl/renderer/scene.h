#ifndef _SCENE_H_
#define _SCENE_H_

#include "camera.h"
#include "model.h"
#include "shader.h"

typedef struct _sn {
  struct _sn *next;
  Model models[1024];
} SceneNode;

typedef struct {
  ShaderProgram shader;
} SceneSkybox;

typedef struct {
  Camera camera;
  ShaderProgram *shader;
  SceneSkybox skybox;
  SceneNode *head;
} Scene;

void scene_init(Scene *scene);
void scene_draw(Scene *scene);
void scene_skybox_init(Scene *scene);

#endif