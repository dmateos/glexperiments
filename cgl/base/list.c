#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

List list_create(void) {
  List list = {.head = NULL, .size = 0};
  return list;
}

void list_destroy(List *list) {
  ListNode *node = list->head;
  while (node != NULL) {
    ListNode *next = node->next;
    printf("freeing node 0x%p\n", node);
    free(node);
    node = next;
  }
}

void list_push(List *list, void *data) {
  ListNode *node = (ListNode *)malloc(sizeof(ListNode));
  node->data = data;
  node->next = list->head;
  list->head = node;
  printf("pushed node 0x%p, head now 0x%p\n", node, list->head);
}

void *list_pop(List *list) {
  ListNode *node = list->head;
  if (node == NULL) {
    return NULL;
  }
  list->head = node->next;
  void *data = node->data;
  free(node);
  return data;
}

void list_test(void) {
  const char test_strings[][6] = {"test1", "test2", "test3"};
  int i = 0;

  List list = list_create();
  list_push(&list, (void *)"test3");
  list_push(&list, (void *)"test2");
  list_push(&list, (void *)"test1");

  list_for_each(list) {
    printf("data is: %s\n", (char *)node->data);
    assert(strcmp((char *)node->data, test_strings[i++]) == 0);
  }

  list_destroy(&list);
}