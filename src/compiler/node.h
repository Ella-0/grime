struct Node {
  const char *data;
  int childCount;
  struct Node *children;
};

struct Node tld;

struct Node createNode(const char *data, int childCount, struct Node *node);
void printNode(struct Node node);
