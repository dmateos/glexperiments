#ifndef _VERTEX_H
#define _VERTEX_H

#define VERTEX_BUFFER_TYPE_ARRAY 0
#define VERTEX_BUFFER_TYPE_INDEX 1

typedef struct VB {
    unsigned int vbo;
    unsigned char type;
    unsigned int length;
} VertexBuffer;

#define VERTEX_STATE_DRAW_ARRAY 0
#define VERTEX_STATE_DRAW_INDEXED 1
typedef struct VS {
    unsigned int vao;
    unsigned char draw_type;
} VertexState;

int init_vertex_state(VertexState *, unsigned char);
int init_vertex_buffer(VertexBuffer *, unsigned char);

void bind_vertex_state(const VertexState *);
void bind_vertex_buffer(const VertexBuffer *);
void write_vertex_buffer(VertexBuffer *, void *, int);

void draw(const VertexState *, int);
#endif