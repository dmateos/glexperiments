#ifndef _VERTEX_H
#define _VERTEX_H

typedef struct VB {
} VertexBuffer;

typedef struct VS {
} VertexState;

int init_vertex_state(VertexState*);
int init_vertex_buffer(VertexBuffer*);

#endif