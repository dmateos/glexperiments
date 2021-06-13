#ifndef _VERTEX_H
#define _VERTEX_H

#define VERTEX_BUFFER_TYPE_ARRAY 0
#define VERTEX_BUFFER_TYPE_INDEX 1

typedef struct VB {
    unsigned int vbo;
    unsigned char type;
} VertexBuffer;

typedef struct VS {
    unsigned int vao;
} VertexState;

int init_vertex_state(VertexState *);
int init_vertex_buffer(VertexBuffer *, unsigned char);

void bind_vertex_state(const VertexState *);
void bind_vertex_buffer(const VertexBuffer *);
void write_vertex_buffer(VertexBuffer *, void *, int);

void draw_array(int);
#endif