enum NodeType {
  NTLD,
  NFUNC,
  NARGS,
  NARG,
  NID,
  NTYPE,
  NBLK,
  NEXPR,
  NRETEXPR,
  NINTEXPR,
  NADDEXPR,
  NMULEXPR,
  NSUBEXPR
};

struct Node {
  enum NodeType type;
  struct Token data;
  int childCount;
  struct Node *children;
};
