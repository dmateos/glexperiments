#include "vertex.h"

#include <OpenGL/gl.h>
#include <stdio.h>
#include <string.h>

int init_vertex_state(VertexState *state) {
    memset(state, 0, sizeof(VertexState));
    glGenVertexArraysAPPLE(1, &state->vao);
    return 0;
}

int init_vertex_buffer(VertexBuffer *buffer) {
    memset(buffer, 0, sizeof(VertexBuffer));
    glGenBuffers(1, &buffer->vbo);
    return 0;
}

void bind_vertex_state(const VertexState *state) {
    glBindVertexArrayAPPLE(state->vao);
    return;
}

void bind_vertex_buffer(const VertexBuffer *buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    return;
}

void draw_array(const VertexState *state) {
    printf("draw\n");
    printf("draw\n");
}