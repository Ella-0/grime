#include "node.c"
#include "match.c"

struct Node parse(struct Token *src);
struct Node parseTLD(struct Token **src);
struct Node parseFunc(struct Token **src);
struct Node parseArgs(struct Token **src);
struct Node parseArg(struct Token **src);
struct Node parseType(struct Token **src);
struct Node parseId(struct Token **src);
struct Node parseBlk(struct Token **src);
struct Node parseExpr(struct Token **src);
struct Node parseRootExpr(struct Token **src);
struct Node parseRetExpr(struct Token **src);
struct Node parseIntExpr(struct Token **src);
struct Node parseAddExpr(struct Token **src);
struct Node parseMulExpr(struct Token **src);
struct Node parseGRTExpr(struct Token **src);

struct Node *appendNode(struct Node *nodes, int *childCount, struct Node node) {
  struct Node *out = (struct Node *) malloc((*childCount + 1) * sizeof(struct Node));
  if (nodes != NULL) {
    memcpy(out, nodes, *childCount * sizeof(struct Node));
    free(nodes);
  }
  out[*childCount] = node;
  (*childCount)++;
  return out;
}

struct Node parse(struct Token *src) {
  return parseTLD(&src);
}

struct Node parseTLD(struct Token **src) {
  struct Node out;
  out.type = NTLD;
  out.childCount = 0;
  out.children = NULL;
  while ((*src)->type) {
    const char *data = (*src)->data;
    if (!strcmp(data, "func")) {
      out.children = appendNode(out.children, &out.childCount, parseFunc(src));
    } else {
      printf("\x1B[31m\x1B[1munexpected token: %s\n\x1B[0m", data);
      exit(-1);
    }
  }
  return out;
}

struct Node parseFunc(struct Token **src) {
  struct Node out;
  out.type = NFUNC;
  out.data = **src;
  (*src)++; //func
  out.childCount = 4;
  struct Node idNode = parseId(src);
  struct Node argsNode = parseArgs(src);
  (*src)++; //->
  struct Node typeNode = parseType(src);
  struct Node blkNode = parseBlk(src);
  out.children = (struct Node *)malloc(out.childCount * sizeof(struct Node));
  out.children[0] = idNode;
  out.children[1] = argsNode;
  out.children[2] = typeNode;
  out.children[3] = blkNode;
  return out;
}


struct Node parseArgs(struct Token **src) {
  struct Node out;
  out.type = NARGS;
  out.childCount = 0;
  out.children = NULL;
  (*src)++; //(
  while (strcmp((*src)->data, ")")) {
    out.children = appendNode(out.children, &out.childCount, parseArg(src));
  }
  (*src)++; //)
  return out;
}

struct Node parseArg(struct Token **src) {
  struct Node out;
  out.type = NARG;
  out.childCount = 2;
  struct Node idNode = parseId(src);
  (*src)++; //:
  struct Node typeNode = parseType(src);
  out.children = (struct Node *) malloc(out.childCount * sizeof(struct Node));
  out.children[0] = idNode;
  out.children[1] = typeNode;
  return out;
}

struct Node parseType(struct Token **src) {
  struct Node out = parseId(src);
  out.type = NTYPE;
  return out;
}

struct Node parseId(struct Token **src) {
  struct Node out;
  out.type = NID;
  out.data = **src;
  out.childCount = 0;
  (*src)++; //<id>
  return out;
}

struct Node parseBlk(struct Token **src) {
  struct Node out;
  out.type = NBLK;
  out.childCount = 0;
  out.children = NULL;
  (*src)++; //{
  while (strcmp((*src)->data, "}")) {
    out.children = appendNode(out.children, &out.childCount, parseExpr(src));
  }
  (*src)++; //}
  return out;
}

struct Node parseExpr(struct Token **src) {
  struct Node out;
  out.type = NEXPR;
  out.childCount = 1;
  out.children = (struct Node *) malloc(sizeof(struct Node));
  out.children[0] = parseGRTExpr(src);
  return out;
}

struct Node parseRootExpr(struct Token **src) {
  if (!strcmp((*src)->data, "return")) {
    return parseRetExpr(src);
  } else if (!strcmp((*src)->data, "(")) {
    (*src)++;//(
    struct Node out = parseExpr(src);
    (*src)++;//)
    return parseExpr(src);
  } else if (isInteger(**src)) {
    return parseIntExpr(src);
  } else {
    printf("unrecognised token when parsing root expression [%s]\n", (*src)->data);
    exit(-1);
  }
}

struct Node parseIntExpr(struct Token **src) {
  struct Node out;
  out.type = NINTEXPR;
  out.data = **src;
  out.childCount = 0;
  (*src)++; //<int>
  return out;
}

struct Node parseRetExpr(struct Token **src) {
  struct Node out;
  out.type = NRETEXPR;
  out.data = **src;
  (*src)++; //return
  out.childCount = 1;
  out.children = (struct Node *) malloc(out.childCount * sizeof(struct Node));
  out.children[0] = parseExpr(src);
  return out;
}

struct Node parseMulExpr(struct Token **src) {
  if (!strcmp((*src+1)->data, "*")) {
    struct Node out;
    out.type = NMULEXPR;
    out.childCount = 2;
    out.children = (struct Node *) malloc(out.childCount * sizeof(struct Node));
    out.children[0] = parseRootExpr(src);
    (*src)++; //*
    out.children[1] = parseExpr(src);
    return out;
  } else if (!strcmp((*src+1)->data, "/")) {
    struct Node out;
    out.type = NDIVEXPR;
    out.childCount = 2;
    out.children = (struct Node *) malloc(out.childCount * sizeof(struct Node));
    out.children[0] = parseRootExpr(src);
    (*src)++; ///
    out.children[1] = parseExpr(src);
    return out;
  } else {
    return parseRootExpr(src);
  }
}

struct Node parseAddExpr(struct Token **src) {
  if (!strcmp((*src+1)->data, "+")) {
    struct Node out;
    out.type = NADDEXPR;
    out.childCount = 2;
    out.children = (struct Node *) malloc(out.childCount * sizeof(struct Node));
    out.children[0] = parseMulExpr(src);
    (*src)++; //+
    out.children[1] = parseExpr(src);
    return out;
  } else if (!strcmp((*src+1)->data, "-")) {
    struct Node out;
    out.type = NSUBEXPR;
    out.childCount = 2;
    out.children = (struct Node *) malloc(out.childCount * sizeof(struct Node));
    out.children[0] = parseMulExpr(src);
    (*src)++; //-
    out.children[1] = parseExpr(src);
    return out;
  } else {
    return parseMulExpr(src);
  }
}

struct Node parseGRTExpr(struct Token **src) {
  if (!strcmp((*src+1)->data, ">")) {
    struct Node out;
    out.type = NGRTEXPR;
    out.childCount = 2;
    out.children = (struct Node *) malloc(out.childCount * sizeof(struct Node));
    out.children[0] = parseAddExpr(src);
    (*src)++; //>
    out.children[1] = parseExpr(src);
    return out;
  } else if (!strcmp((*src+1)->data, "<")) {
    struct Node out;
    out.type = NLSTEXPR;
    out.childCount = 2;
    out.children = (struct Node *) malloc(out.childCount * sizeof(struct Node));
    out.children[0] = parseAddExpr(src);
    (*src)++; //<
    out.children[1] = parseExpr(src);
    return out;
  } else if (!strcmp((*src+1)->data, ">=")) {
    struct Node out;
    out.type = NGTEEXPR;
    out.childCount = 2;
    out.children = (struct Node *) malloc(out.childCount * sizeof(struct Node));
    out.children[0] = parseAddExpr(src);
    (*src)++; //>=
    out.children[1] = parseExpr(src);
    return out;
  } else if (!strcmp((*src+1)->data, "<=")) {
    struct Node out;
    out.type = NLTEEXPR;
    out.childCount = 2;
    out.children = (struct Node *) malloc(out.childCount * sizeof(struct Node));
    out.children[0] = parseAddExpr(src);
    (*src)++; //<=
    out.children[1] = parseExpr(src);
    return out;
  } else {
    return parseMulExpr(src);
  }
}
