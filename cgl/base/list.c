#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

list_t list_create(void) {
  list_t list;
  list.head = NULL;
  return list;
}

void list_destroy(list_t *list) {
  list_node_t *node = list->head;
  while (node != NULL) {
    list_node_t *next = node->next;
    free(node);
    node = next;
  }
}

void list_push(list_t *list, void *data) {
  list_node_t *node = malloc(sizeof(list_node_t));
  node->data = data;
  node->next = list->head;
  list->head = node;
}

void *list_pop(list_t *list) {
  list_node_t *node = list->head;
  if (node == NULL) {
    return NULL;
  }
  list->head = node->next;
  void *data = node->data;
  free(node);
  return data;
}

void test_list(void) {
  char test_strings[][6] = {"test1", "test2", "test3"};
  int i = 0;

  list_t list = list_create();
  list_push(&list, (void *)"test3");
  list_push(&list, (void *)"test2");
  list_push(&list, (void *)"test1");

  list_for_each(list) {
    printf("data is: %s\n", (char *)node->data);
    assert(strcmp((char *)node->data, test_strings[i++]) == 0);
  }

  list_destroy(&list);
}