#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 100

typedef struct {
  int64_t value;
  int32_t left;
  int32_t right;
} Node;

typedef struct {
  Node nodes[MAX_NODES];
  int32_t root;
  int32_t free_head;
} BinaryTree;

BinaryTree *new_binary_tree() {
  BinaryTree *tree = (BinaryTree *)malloc(sizeof(BinaryTree));

  for (int32_t i = 0; i < MAX_NODES - 1; i++) {
    tree->nodes[i].left = i + 1;
  }
  tree->nodes[MAX_NODES - 1].left = -1;

  tree->root = -1;
  tree->free_head = 0;

  return tree;
}

int32_t get_index(BinaryTree *tree) {
  if (tree->free_head == -1) {
    printf("All is lost");
    return -1;
  }

  int32_t index = tree->nodes[tree->free_head].left;
  tree->free_head = tree->nodes[index].left;
  return index;
}

void insert(BinaryTree *tree, int64_t value) {
  int32_t index = get_index(tree);
  if (index == -1) {
    printf("No space for insertion");
    return;
  }

  tree->nodes[index].value = value;
  tree->nodes[index].left = -1;
  tree->nodes[index].right = -1;

  if (tree->root == -1) {
    tree->root = index;
    return;
  }

  int32_t temp_root = tree->root;
  while (true) {
    if (tree->nodes[temp_root].value > value) {
      if (tree->nodes[temp_root].left == -1) {
        tree->nodes[temp_root].left = index;
        break;
      }
      temp_root = tree->nodes[temp_root].left;
    } else {
      if (tree->nodes[temp_root].right == -1) {
        tree->nodes[temp_root].right = index;
        break;
      }
      temp_root = tree->nodes[temp_root].right;
    }
  }
}

void preorder(BinaryTree *tree, int32_t index) {
  if (index == -1) {
    return;
  }
  preorder(tree, tree->nodes[index].left);
  printf("Value at %d: %ld\n", index, tree->nodes[index].value);
  preorder(tree, tree->nodes[index].right);
}

void free_node(BinaryTree *tree, int32_t index) {
  tree->nodes[index].left = tree->free_head;
  tree->free_head = index;
}

int32_t find_min(BinaryTree *tree, int32_t index) {
  while (tree->nodes[index].left != -1) {
    index = tree->nodes[index].left;
  }
  return index;
}

int32_t delete_impl(BinaryTree *tree, int32_t index, int64_t value) {
  if (index == -1) {
    return -1;
  }
  if (tree->nodes[index].value > value) {
    tree->nodes[index].left = delete_impl(tree, tree->nodes[index].left, value);
  } else if (tree->nodes[index].value < value) {
    tree->nodes[index].right =
        delete_impl(tree, tree->nodes[index].right, value);
  } else {
    if (tree->nodes[index].left == -1 && tree->nodes[index].right == -1) {
      free_node(tree, index);
      return -1;
    }
    if (tree->nodes[index].left == -1) {
      int32_t ri = tree->nodes[index].right;
      free_node(tree, index);
      return ri;
    } else if (tree->nodes[index].right == -1) {
      int32_t li = tree->nodes[index].left;
      free_node(tree, index);
      return li;
    } else {
      int32_t min_index = find_min(tree, tree->nodes[index].right);
      tree->nodes[index].value = tree->nodes[min_index].value;
      tree->nodes[index].right = delete_impl(tree, tree->nodes[index].right,
                                             tree->nodes[min_index].value);
    }
  }
  return index;
}

void delete_value(BinaryTree *tree, int64_t value) {
  tree->root = delete_impl(tree, tree->root, value);
}
void free_tree(BinaryTree *tree) { free(tree); }

int main() {
  BinaryTree *tree = new_binary_tree();
  insert(tree, 29);
  insert(tree, 222);
  insert(tree, 200);
  insert(tree, 220);
  insert(tree, 229);
  insert(tree, 12);
  insert(tree, 22);
  insert(tree, 10);
  delete_value(tree, 222);
  preorder(tree, tree->root);

  free_tree(tree);
  return 0;
}
