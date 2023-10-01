#include "scene.h"
#include "camera.h"
#include "shader.h"

void scene_init(Scene *scene) {
  shader_program_init(&scene->shader);
  shader_program_add(&scene->shader, VERTEXSHADER, "renderer/shaders/vert.gsl");
  shader_program_add(&scene->shader, FRAGSHADER, "renderer/shaders/frag.gsl");
  shader_program_compile(&scene->shader);
  shader_use(&scene->shader);

  camera_init(&scene->camera, &scene->shader, 1280 / 1024);

  model_skybox_init(&scene->skybox, "assets/textures/skybox/");
  camerea_update_uniforms(&scene->camera, &scene->skybox.program);

  model_init(&scene->test_model, &scene->shader, "assets/models/cube.obj",
             "assets/textures/gridtex.png", 0, NULL);
  printf("scene initialized\n");
}

void scene_update(Scene *scene) {
  shader_use(&scene->skybox.program);
  camerea_update_uniforms(&scene->camera, &scene->skybox.program);
}

void scene_draw(Scene *scene) {
  model_skybox_draw(&scene->skybox);
  model_draw(&scene->test_model);
}

void scene_free(Scene *scene) {
  model_free(&scene->test_model);
  printf("scene freed\n");
}