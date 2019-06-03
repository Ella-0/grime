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
  NSUBEXPR,
  NDIVEXPR,
  NGRTEXPR,
  NLSTEXPR,
  NGTEEXPR,
  NLTEEXPR
};

struct Node {
  enum NodeType type;
  struct Token data;
  int childCount;
  struct Node *children;
};
