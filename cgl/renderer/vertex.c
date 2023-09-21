#include "vertex.h"
#include "shader.h"

#include <OpenGL/gl3.h>
#include <stdio.h>
#include <string.h>

int vertex_init_state(VertexState *state, DrawType type) {
  memset(state, 0, sizeof(VertexState));
  glGenVertexArrays(1, &state->vao);
  state->draw_type = type;
  printf("initiated new vertex state %u\n", state->vao);
  return 0;
}

int vertex_init_buffer(VertexBuffer *buffer, BufferType type,
                       int8_t instanced) {
  memset(buffer, 0, sizeof(VertexBuffer));
  buffer->type = type;
  buffer->instanced = instanced;
  glGenBuffers(1, &buffer->vbo);

  if (buffer->instanced) {
    printf("buffer is instanced\n");
  }
  printf("initiated new vertex buffer %u\n", buffer->vbo);
  return 0;
}

void vertex_bind_state(const VertexState *state) {
  glBindVertexArray(state->vao);
  // printf("bound vertex state %u\n", state->vao);
}

void vertex_unbind_state(const VertexState *state) { glBindVertexArray(0); }

void vertex_bind_buffer(const VertexBuffer *buffer) {
  switch (buffer->type) {
  case VERTEX_BUFFER_TYPE_ARRAY:
    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    break;
  case VERTEX_BUFFER_TYPE_INDEX:
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->vbo);
    break;
  }
  // printf("bound vertex buffer %u\n", buffer->vbo);
}

void vertex_unbind_buffer(const VertexBuffer *buffer) {
  switch (buffer->type) {
  case VERTEX_BUFFER_TYPE_ARRAY:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    break;
  case VERTEX_BUFFER_TYPE_INDEX:
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    break;
  }
}

void vertex_write_buffer(VertexBuffer *buffer, void *data, int32_t size) {
  switch (buffer->type) {
  case VERTEX_BUFFER_TYPE_ARRAY: {
    printf("wrote vertex array buffer data with size %d\n", size);
    float *fptr = (float *)data;
    for (int i = 0; i < size / (int)sizeof(float); i++) {
      printf("(%d %f) ", i, *fptr);
      fptr++;
    }
    printf("\n");
    glBufferData(GL_ARRAY_BUFFER, size, (void *)data, GL_STATIC_DRAW);
  } break;
  case VERTEX_BUFFER_TYPE_INDEX: {
    printf("wrote vertex index buffer data with size %d\n", size);
    int32_t *iptr = (int32_t *)data;
    for (int i = 0; i < size / (int)sizeof(int); i++) {
      printf("(%d %d) ", i, *iptr + 1);
      iptr++;
    }
    printf("\n");
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, (void *)data, GL_STATIC_DRAW);
    break;
  }
  }
  buffer->length = size;
}

void vertex_draw(const VertexState *state, int32_t length) {
  vertex_bind_state(state);
  switch (state->draw_type) {
  case VERTEX_STATE_DRAW_ARRAY:
    glDrawArrays(GL_TRIANGLES, 0, length);
    break;
  case VERTEX_STATE_DRAW_INDEXED:
    glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT, NULL);
    break;
  }
  // printf("drawing %d with %d length\n", state->vao, length);
}

void vertex_draw_instanced(const VertexState *state, int32_t length,
                           int32_t count) {
  vertex_bind_state(state);
  switch (state->draw_type) {
  case VERTEX_STATE_DRAW_ARRAY:
    printf("not implemented\n");
    break;
  case VERTEX_STATE_DRAW_INDEXED:
    glDrawElementsInstanced(GL_TRIANGLES, length, GL_UNSIGNED_INT, NULL, count);
    break;
  }
}

void vertex_get_error(void) {
  int32_t error = glGetError();
  if (error == 0) {
    printf("NO GL ERRORS\n");
  } else {
    printf("%d\n", error);
  }
}
