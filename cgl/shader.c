#include "shader.h"

#include <OpenGL/gl.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

int init_shaderprogram(ShaderProgram *program) {
    memset(program, 0, sizeof(ShaderProgram));

    program->program_id = glCreateProgram();
    printf("created new GL shader program\n");
    return 0;
}

void use_shaderprogram(const ShaderProgram *program) {
    glUseProgram(program->program_id);
    printf("using shader program\n");
}

int destroy_shaderprogram(ShaderProgram *program) { return 0; }

void add_shader(ShaderProgram *program, int type, char *filepath) {
    unsigned int shader_id;
    char *shader_data = read_file(filepath);

    if (!shader_data) {
        assert("could not load shader data");
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

    // glShaderSource(shader_id, 1, code, strlen(code));
    program->shaders[program->shader_count++].shader_id = shader_id;

    free_file_data(shader_data);
    printf("added new shader to program in slot %d\n",
           program->shader_count - 1);
}