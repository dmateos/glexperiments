#ifndef _VERTEX_H
#define _VERTEX_H

typedef struct VB {
    unsigned int vbo;
} VertexBuffer;

typedef struct VS {
    unsigned int vao;
} VertexState;

int init_vertex_state(VertexState *);
int init_vertex_buffer(VertexBuffer *);

void bind_vertex_state(const VertexState *);
void bind_vertex_buffer(const VertexBuffer *);
void write_vertex_buffer(VertexBuffer *, void *, int);

void draw_array(int);
#endif