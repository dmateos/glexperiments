#ifndef _VERTEX_H
#define _VERTEX_H

#include <stdint.h>

typedef enum { VERTEX_BUFFER_TYPE_ARRAY, VERTEX_BUFFER_TYPE_INDEX } BufferType;
typedef enum { VERTEX_STATE_DRAW_ARRAY, VERTEX_STATE_DRAW_INDEXED } DrawType;

typedef struct {
  uint32_t vbo;
  BufferType type;
  uint32_t length;
  int8_t instanced;
} VertexBuffer;

typedef struct {
  uint32_t vao;
  DrawType draw_type;
} VertexState;

int vertex_init_state(VertexState *, DrawType);
int vertex_init_buffer(VertexBuffer *, BufferType, int8_t);

void vertex_bind_state(const VertexState *);
void vertex_unbind_state(const VertexState *);
void vertex_bind_buffer(const VertexBuffer *);
void vertex_unbind_buffer(const VertexBuffer *);
void vertex_write_buffer(VertexBuffer *, void *, int32_t);

void vertex_draw(const VertexState *, int32_t);
void vertex_draw_instanced(const VertexState *, int32_t, int32_t);

void vertex_get_error(void);
#endif
