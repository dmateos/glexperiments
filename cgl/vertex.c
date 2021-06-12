#include "vertex.h"

#include <stdio.h>
#include <string.h>

int init_vertex_state(VertexState *state) {
    memset(state, 0, sizeof(VertexState));
    return 0;
}

int init_vertex_buffer(VertexBuffer *buffer) {
    memset(buffer, 0, sizeof(VertexBuffer));
    return 0;
}