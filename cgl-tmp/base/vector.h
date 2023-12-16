#ifndef _VECTOR_H_
#define _VECTOR_H_

// cpp like vector
typedef struct {
  void **data;
  int size;
  int capacity;
} Vector;

void vector_init(Vector *vector);
void vector_destroy(Vector *vector);
void vector_push(Vector *vector, void *value);
Vector *vector_pop(Vector *vector);

#define vector_foreach(vector, iterator)                                       \
  for (int iterator = 0; iterator < vector->size; iterator++)
#endif