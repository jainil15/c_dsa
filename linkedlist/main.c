#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_NODES 100
typedef struct {
  int64_t value;
  int32_t next_index;
} Node;

typedef struct {
  Node *nodes;
  int32_t head;
  int32_t last_index;
  int32_t tail;
} LinkedList;

LinkedList* new() {
  LinkedList *linked_list = malloc(sizeof(LinkedList));
  linked_list->nodes = malloc(sizeof(Node) * MAX_NODES);
  linked_list->head = -1;
  linked_list->tail = -1;
  linked_list->last_index = -1;
  return linked_list;
}

void insert_first(LinkedList *list, int64_t value) {
  if (list->last_index >= MAX_NODES - 1) {
    printf("\nExceeds the MAX NODES\n");
    return;
  }
  list->last_index++;
  list->nodes[list->last_index].value = value;
  if (list->head == -1) {
    list->nodes[list->last_index].next_index = -1;
    list->tail = list->last_index;
  } else {
    list->nodes[list->last_index].next_index = list->head;
  }
  list->head = list->last_index;
}

void insert_last(LinkedList *list, int64_t value) {
  if (list->last_index >= MAX_NODES - 1) {
    printf("\nExceeds the MAX NODES\n");
    return;
  }
  list->last_index++;
  list->nodes[list->last_index].next_index = -1;
  list->nodes[list->last_index].value = value;
  if (list->head == -1) {
    list->head = list->last_index;
    list->tail = list->last_index;
  } else {
    list->nodes[list->tail].next_index = list->last_index;
    list->tail = list->last_index;
  }

}

void free_list(LinkedList *list) {
  free(list->nodes);
  free(list);
}


void print_list(LinkedList *list) {
  int32_t current_index = 0;
  int32_t temp_head = list->head;
  printf("\n");
  while(temp_head != -1) {
    printf("Node %d: %ld\n", temp_head, list->nodes[temp_head].value);
    current_index++;
    temp_head = list->nodes[temp_head].next_index;
  }
}



int main() {
  LinkedList *list = new();
  insert_last(list, 40);
  insert_first(list, 10);
  insert_last(list, 90);
  insert_last(list, 50);
  insert_first(list, 20);
  insert_last(list, 40);
  print_list(list);
  free_list(list);
  return 0;
}
