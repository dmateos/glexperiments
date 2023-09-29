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

  scene_skybox_init(scene);

  model_init(&scene->test_model, &scene->shader, "assets/models/cube.obj", 0,
             NULL);
  texture_init(&scene->texture, "assets/textures/gridtex.png");

  scene->test_model.vec[2] += 3.0;
  scene->test_model.vec[0] += 3.0;
}

void scene_draw(Scene *scene) { model_draw(&scene->test_model); }

void scene_skybox_init(Scene *scene) {}