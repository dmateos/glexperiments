#ifndef _SHADER_H
#define _SHADER_H

#define VERTEXSHADER 0
#define FRAGSHADER 1

typedef struct Shader {
} Shader;

typedef struct Program {
    unsigned int program_id;
    Shader shaders[16];
    int shader_count;
} ShaderProgram;

int init_shaderprogram(ShaderProgram *);
int destroy_shaderprogram(ShaderProgram *);
void use_shaderprogram(const ShaderProgram *);
void add_shader(ShaderProgram *, int, char *);

#endif