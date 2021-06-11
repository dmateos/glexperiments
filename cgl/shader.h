#ifndef _SHADER_H
#define _SHADER_H

typedef struct Program {
    unsigned int program_id;
} ShaderProgram;

typedef struct Shader {
} Shader;

ShaderProgram *init_shaderprogram();
int destroy_shaderprogram();

#endif