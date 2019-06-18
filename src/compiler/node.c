#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "node.h"

// struct Node createNode(const char *data, int childCount, ...) {
//   va_list ap;
//   va_start(ap, childCount);
//   struct Node *children = (struct Node *) malloc(childCount * sizeof(struct Node));
//   for (int i = 0; i < childCount; i++) {
//     children[i] = va_arg(ap, struct Node);
//     printf("  %s\n", children[i]);
//   }
//   printf("NODE %s %d\n", data, childCount);
//   struct Node out;
//   out.data = data;
//   out.childCount = childCount;
//   out.children = children;
//   return out;
// }

struct Node createNode(char *data, int childCount, struct Node *node) {
  struct Node *children = (struct Node *) malloc(childCount * sizeof(struct Node));
  for (int i = 0; i < childCount; i++) {
    children[i] = node[i];
    printf("  %s %d\n", children[i].data, children[i].childCount);
  }
  printf("NODE %s %d\n", data, childCount);
  struct Node out;
  out.data = data;
  out.childCount = childCount;
  out.children = children;
  return out;
}

void printSubNode(struct Node node, int tabs) {
  for (int i = 0; i < tabs; i++) {
    printf("  ");
  }
  printf("%s -- %d -\\\n", node.data, node.childCount);
  for (int i = 0; i < node.childCount; i++) {
    printSubNode(node.children[i], tabs + 1);
  }
}

void printNode(struct Node node) {
  printSubNode(node, 0);
}
