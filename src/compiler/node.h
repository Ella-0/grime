struct Node {
  char *data;
  int childCount;
  struct Node *children;
};

struct Node tld;

struct Node createNode(char *data, int childCount, struct Node *node);
void printNode(struct Node node);
