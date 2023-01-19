#ifndef _VERTEX_H
#define _VERTEX_H

#include <stdint.h>

typedef enum buffertype {
    VERTEX_BUFFER_TYPE_ARRAY,
    VERTEX_BUFFER_TYPE_INDEX
} BufferType;

typedef struct VB {
    uint32_t vbo;
    BufferType type;
    uint32_t length;
    int8_t instanced;
} VertexBuffer;

typedef enum drawtype {
    VERTEX_STATE_DRAW_ARRAY,
    VERTEX_STATE_DRAW_INDEXED
} DrawType;

typedef struct VS {
    uint32_t vao;
    DrawType draw_type;
} VertexState;

int init_vertex_state(VertexState *, DrawType);
int init_vertex_buffer(VertexBuffer *, BufferType, int8_t);

void bind_vertex_state(const VertexState *);
void unbind_vertex_state(const VertexState *);
void bind_vertex_buffer(const VertexBuffer *);
void unbind_vertex_buffer(const VertexBuffer *);
void write_vertex_buffer(VertexBuffer *, void *, int32_t);

void draw(const VertexState *, int32_t);
void draw_instanced(const VertexState *, int32_t, int32_t);

void get_error(void);
#endif
