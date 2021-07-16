#ifndef _VERTEX_H
#define _VERTEX_H

typedef enum buffertype {
    VERTEX_BUFFER_TYPE_ARRAY,
    VERTEX_BUFFER_TYPE_INDEX
} BufferType;

typedef struct VB {
    unsigned int vbo;
    BufferType type;
    unsigned int length;
    char instanced;
} VertexBuffer;

typedef enum drawtype {
    VERTEX_STATE_DRAW_ARRAY,
    VERTEX_STATE_DRAW_INDEXED
} DrawType;

typedef struct VS {
    unsigned int vao;
    DrawType draw_type;
} VertexState;

int init_vertex_state(VertexState *, DrawType);
int init_vertex_buffer(VertexBuffer *, BufferType, char);

void bind_vertex_state(const VertexState *);
void unbind_vertex_state(const VertexState *);
void bind_vertex_buffer(const VertexBuffer *);
void unbind_vertex_buffer(const VertexBuffer *);
void write_vertex_buffer(VertexBuffer *, void *, int);

void draw(const VertexState *, int);
void draw_instanced(const VertexState *, int, int);

void get_error(void);
#endif