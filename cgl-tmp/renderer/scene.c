#include "scene.h"
#include "camera.h"
#include "shader.h"

void scene_init(Scene *scene, int ratio) {
  camera_init(&scene->camera, ratio);
  model_skybox_init(&scene->skybox, "assets/textures/skybox/");
  model_shader_init(&scene->shader, 0);

  model_init(&scene->test_model, &scene->shader, "assets/models/cube.obj",
             "assets/textures/gridtex.png", 0, NULL);
  printf("scene initialized\n");
}

void scene_update(Scene *scene) {
  camera_update(&scene->camera, &scene->skybox.program, 1);
  camera_update(&scene->camera, &scene->shader, 0);
}

void scene_draw(Scene *scene) {
  model_skybox_draw(&scene->skybox);
  model_draw(&scene->test_model);
}

void scene_free(Scene *scene) {
  model_free(&scene->test_model);
  printf("scene freed\n");
}