#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_NODES 100

typedef struct {
  int64_t value;
  int32_t next_index;
} Node;

typedef struct {
  Node *nodes;
  int32_t head;
  int32_t tail;
  int32_t free_head;
} LinkedList;

LinkedList *new_list() {
  LinkedList *list = malloc(sizeof(LinkedList));
  list->nodes = (Node *)malloc(sizeof(Node) * MAX_NODES);

  for (int32_t i = 0; i < MAX_NODES - 1; i++) {
    list->nodes[i].next_index = i + 1;
  }
  list->nodes[MAX_NODES - 1].next_index = -1;

  list->head = -1;
  list->tail = -1;
  list->free_head = 0;

  return list;
}

int32_t get_index(LinkedList *list) {
  if (list->free_head == -1) {
    return -1;
  };
  int32_t index = list->free_head;
  list->free_head = list->nodes[index].next_index;
  return index;
}

void insert_first(LinkedList *list, int64_t value) {
  int32_t i = get_index(list);
  if (i == -1) {
    printf("No available space");
  }
  list->nodes[i].value = value;
  if (list->head == -1) {
    list->nodes[i].next_index = -1;
    list->tail = i;
  } else {
    list->nodes[i].next_index = list->head;
  }
  list->head = i;
}

void insert_last(LinkedList *list, int64_t value) {
  int32_t i = get_index(list);
  if (i == -1) {
    printf("No available space");
  }
  list->nodes[i].next_index = -1;
  list->nodes[i].value = value;
  if (list->head == -1) {
    list->head = i;
    list->tail = i;
  } else {
    list->nodes[list->tail].next_index = i;
    list->tail = i;
  }
}

void insert_at(LinkedList *list, int64_t value, int32_t index) {

  int32_t i = get_index(list);
  if (i == -1) {
    printf("No available space");
  }

  if (index == 0) {
    insert_first(list, value);
    return;
  }

  int32_t temp_head = list->head;
  int32_t current_index = 1;

  while (current_index < index && list->nodes[temp_head].next_index != -1) {
    current_index++;
    temp_head = list->nodes[temp_head].next_index;
  }

  if (current_index != index) {
    printf("Cant insert element at this index");
    return;
  }

  int32_t temp_next_index = list->nodes[temp_head].next_index;
  list->nodes[temp_head].next_index = i;
  list->nodes[i].value = value;
  list->nodes[i].next_index = temp_next_index;

  if (temp_next_index == -1) {
    list->tail = i;
  }
}

void delete_first(LinkedList *list) {
  if (list->head == -1) {
    printf("List is empty");
    exit(0);
    return;
  }
  int32_t old_head = list->head;
  list->head = list->nodes[list->head].next_index;
  if (list->head == -1) {
    list->tail = -1;
  }
  list->nodes[old_head].next_index = list->free_head;
  list->free_head = old_head;
}
void delete_last(LinkedList *list) {
  if (list->head == -1) {
    printf("List is empty");
    exit(0);
    return;
  }
  if (list->head == list->tail) {
    int32_t old_tail = list->tail;
    list->head = -1;
    list->tail = -1;
    list->nodes[old_tail].next_index = list->free_head;
    list->free_head = old_tail;
    return;
  }

  int32_t temp_head = list->head;
  while (list->nodes[temp_head].next_index != list->tail) {
    temp_head = list->nodes[temp_head].next_index;
  }

  int32_t old_tail = list->tail;
  list->tail = temp_head;
  list->nodes[list->tail].next_index = -1;
  list->nodes[old_tail].next_index = list->free_head;
  list->free_head = old_tail;
  return;
}

void delete_at(LinkedList *list, int32_t index) {
  if (index == 0) {
    delete_first(list);
    return;
  }

  int32_t temp_head = list->head;
  int32_t current_index = 1;

  while (current_index < index && list->nodes[temp_head].next_index != -1) {
    current_index++;
    temp_head = list->nodes[temp_head].next_index;
  }

  if (list->nodes[temp_head].next_index == -1) {
    printf("Invalid index");
    return;
  }

  if (list->nodes[temp_head].next_index == list->tail) {
    delete_last(list);
    return;
  }

  int32_t old_index = list->nodes[temp_head].next_index;

  list->nodes[temp_head].next_index = list->nodes[old_index].next_index;

  list->nodes[old_index].next_index = list->free_head;
  list->free_head = old_index;
}

void free_list(LinkedList *list) {
  free(list->nodes);
  free(list);
}

void print_list(LinkedList *list) {
  int32_t current_index = 0;
  int32_t temp_head = list->head;
  printf("\n");
  while (temp_head != -1) {
    printf("Node %d: %ld\n", temp_head, list->nodes[temp_head].value);
    current_index++;
    temp_head = list->nodes[temp_head].next_index;
  }
}

int64_t get_element(LinkedList *list, int32_t index) {
  if (list->head == -1) {
    printf("List is empty");
    exit(0);
  }
  int32_t temp_head = list->head;
  int32_t current_index = 0;

  while (current_index < index && temp_head != -1) {
    current_index++;
    temp_head = list->nodes[temp_head].next_index;
  }
  if (temp_head == -1) {
    printf("Index out of bound");
    exit(0);
  }
  return list->nodes[temp_head].value;
}

int64_t get_count(LinkedList *list) {
  if (list->head == -1) {
    return 0;
  }

  int32_t temp_head = list->head;
  int32_t count = 0;
  while (temp_head != -1) {
    count++;
    temp_head = list->nodes[temp_head].next_index;
  }
  return count;
}

void clear_list(LinkedList *list) {
  if (list->head == -1) {
    return;
  }
  list->nodes[list->tail].next_index = list->free_head;
  list->free_head = list->head;
  list->head = -1;
  list->tail = -1;
}

int main() {
  LinkedList *list = new_list();
  insert_first(list, 10);
  delete_at(list, 0);
  insert_first(list, 20);
  insert_last(list, 19);
  insert_last(list, 30);
  insert_last(list, 20);
  delete_last(list);
  delete_first(list);
  clear_list(list);
  insert_last(list, 10);
  insert_last(list, 80);
  insert_last(list, 11);
  insert_last(list, 30);
  insert_last(list, 50);
  insert_last(list, 20);
  delete_at(list, 3);
  insert_last(list, 20);
  insert_last(list, 20);
  print_list(list);
  printf("Count: %ld\n", get_count(list));
  free_list(list);
  return 0;
}
