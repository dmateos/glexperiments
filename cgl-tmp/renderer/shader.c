#include "shader.h"

#include <OpenGL/gl3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

// cache for uniforms
static int uniform_cache[1024];
static int uniform_cache_count = 0;

int shader_program_init(ShaderProgram *program) {
  memset(program, 0, sizeof(ShaderProgram));

  program->program_id = glCreateProgram();
  printf("created new GL shader program %d\n", program->program_id);
  return 0;
}

void shader_use(const ShaderProgram *program) {
  glUseProgram(program->program_id);
  // printf("using shader program %d\n", program->program_id);
}

void shader_program_add(ShaderProgram *program, ShaderType type,
                        const char *filepath) {
  unsigned int shader_id;
  const char *shader_data = util_read_file(filepath);
  GLint shader_result = 0;

  if (!shader_data) {
    printf("could not load shader data\n");
    exit(1);
  }

  switch (type) {
  case VERTEXSHADER:
    printf("creating new vertex shader from %s\n", filepath);
    shader_id = glCreateShader(GL_VERTEX_SHADER);
    break;
  case FRAGSHADER:
    printf("creating new fragment shader from %s\n", filepath);
    shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    break;
  }

  glShaderSource(shader_id, 1, &shader_data, NULL);
  glCompileShader(shader_id);
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_result);

  if (!shader_result) {
    printf("could not compile shader %s\n", filepath);
    glDeleteShader(shader_id);
    exit(1);
  }

  program->shaders[program->shader_count++].shader_id = shader_id;

  util_free_file((char *)shader_data);
  printf("added new shader to program %d in slot %d\n", program->program_id,
         program->shader_count - 1);
}

void shader_program_compile(const ShaderProgram *program) {
  for (int i = 0; i < program->shader_count; i++) {
    glAttachShader(program->program_id, program->shaders[i].shader_id);
  }
  glLinkProgram(program->program_id);
  printf("compiled shader program %d\n", program->program_id);
}

void shader_set_attribute(int index, int size, int stride, long offset) {
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, sizeof(float) * stride,
                        (void *)(offset * sizeof(float)));
  printf("set attribute for index %d, size %d, stride %d, offset %ld\n", index,
         size, stride, offset);
}

void shader_set_uniform(int index, float *value) {
  glUniformMatrix4fv(index, 1, GL_FALSE, value);
}

int shader_get_attribute(const ShaderProgram *program, const char *name) {
  return glGetAttribLocation(program->program_id, name);
}

int shader_get_uniform(const ShaderProgram *program, const char *name) {
  return glGetUniformLocation(program->program_id, name);
}
