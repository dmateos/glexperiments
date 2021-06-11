#include "shader.h"

#include <OpenGL/gl.h>
#include <stdlib.h>
#include <string.h>

ShaderProgram *init_shaderprogram() {
    ShaderProgram *program = malloc(sizeof(ShaderProgram));
    memset(program, 0, sizeof(ShaderProgram));

    program->program_id = glCreateProgram();
    return program;
}