#ifndef _LIST_H_
#define _LIST_H_

typedef struct list_node {
  struct list_node *next;
  void *data;
} ListNode;

typedef struct list {
  ListNode *head;
  int size;
} List;

List list_create(void);
void list_destroy(List *);
void list_push(List *, void *);
void *list_pop(List *);
void list_test(void);

// Macro to iterate over a list
#define list_for_each(list)                                                    \
  for (ListNode *node = list.head; node != NULL; node = node->next)

#endif