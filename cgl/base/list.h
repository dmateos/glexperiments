#ifndef _LIST_H_
#define _LIST_H_

typedef struct list_node {
  struct list_node *next;
  void *data;
} list_node_t;

typedef struct list {
  list_node_t *head;
  int size;
} list_t;

list_t list_create(void);
void list_destroy(list_t *);
void list_push(list_t *, void *);
void *list_pop(list_t *);
void test_list(void);

// Macro to iterate over a list
#define list_for_each(list)                                                    \
  for (list_node_t *node = list.head; node != NULL; node = node->next)

#endif