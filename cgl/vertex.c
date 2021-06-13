#include "vertex.h"

#include <OpenGL/gl3.h>
#include <stdio.h>
#include <string.h>

int init_vertex_state(VertexState *state) {
    memset(state, 0, sizeof(VertexState));
    glGenVertexArrays(1, &state->vao);
    printf("initiated new vertex state %u\n", state->vao);
    return 0;
}

int init_vertex_buffer(VertexBuffer *buffer, unsigned char type) {
    memset(buffer, 0, sizeof(VertexBuffer));
    buffer->type = type;
    glGenBuffers(1, &buffer->vbo);
    printf("initiated new vertex buffer %u\n", buffer->vbo);
    return 0;
}

void bind_vertex_state(const VertexState *state) {
    glBindVertexArray(state->vao);
    printf("bound vertex state %u\n", state->vao);
}

void bind_vertex_buffer(const VertexBuffer *buffer) {
    switch (buffer->type) {
        case VERTEX_BUFFER_TYPE_ARRAY:
            glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
            break;
        case VERTEX_BUFFER_TYPE_INDEX:
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->vbo);
            break;
    }
    printf("bound vertex buffer %u\n", buffer->vbo);
}

void write_vertex_buffer(VertexBuffer *buffer, void *data, int size) {
    glBufferData(GL_ARRAY_BUFFER, size, (void *)data, GL_STATIC_DRAW);
    printf("wrote vertex buffer data with size %d\n", size);
}

void draw_array(int length) {
    glDrawArrays(GL_TRIANGLES, 0, length);
    return;
}