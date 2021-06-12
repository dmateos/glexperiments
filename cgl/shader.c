#include "shader.h"

#include <OpenGL/gl.h>
#include <stdlib.h>
#include <string.h>

int init_shaderprogram(ShaderProgram *program) {
    memset(program, 0, sizeof(ShaderProgram));

    program->program_id = glCreateProgram();
    return 0;
}

void use_shaderprogram(const ShaderProgram *program) {
    glUseProgram(program->program_id);
}

int destroy_shaderprogram(ShaderProgram *program) { return 0; }

void add_shader(ShaderProgram *program, int type, char *code) {
    unsigned int shader_id;

    switch (type) {
        case VERTEXSHADER:
            shader_id = glCreateShader(GL_VERTEX_SHADER);
            break;
        case FRAGSHADER:
            shader_id = glCreateShader(GL_FRAGMENT_SHADER);
            break;
    }

    // glShaderSource(shader_id, 1, code, strlen(code));
}