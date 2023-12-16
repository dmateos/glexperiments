#ifndef _SHADER_H
#define _SHADER_H

typedef enum shadertype { VERTEXSHADER, FRAGSHADER } ShaderType;

typedef struct {
  unsigned int shader_id;
} Shader;

typedef struct {
  unsigned int program_id;
  Shader shaders[16];
  int shader_count;
} ShaderProgram;

int shader_program_init(ShaderProgram *);
void shader_use(const ShaderProgram *);
void shader_program_add(ShaderProgram *, ShaderType, const char *);
void shader_program_compile(const ShaderProgram *);
void shader_set_attribute(int, int, int, long);
void shader_set_uniform(int, float *);
int shader_get_attribute(const ShaderProgram *, const char *);
int shader_get_uniform(const ShaderProgram *, const char *);
#endif
