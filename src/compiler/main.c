#include <stdio.h>
#include <stdlib.h>
#include "lexer.c"
#include "parser.c"
#include "codegen.c"

void printTree(struct Node ast, int t) {
  for (int n = t; n; n--) {
    printf(" ");
  }
  switch (ast.type) {
    case NTLD:
    printf("NTLD\n");
    break;

    case NFUNC:
    printf("NFUNC\n");
    break;

    case NARGS:
    printf("NARGS\n");
    break;

    case NARG:
    printf("NARG\n");
    break;

    case NID:
    printf("NID\n");
    break;

    case NTYPE:
    printf("NTYPE\n");
    break;

    case NBLK:
    printf("NBLK\n");
    break;

    case NEXPR:
    printf("NEXPR\n");
    break;

    case NRETEXPR:
    printf("NRETEXPR\n");
    break;

    case NINTEXPR:
    printf("NINTEXPR\n");
    break;

    case NADDEXPR:
    printf("NADDEXPR\n");
  }
  for (int i = 0; i < ast.childCount; i++) {
    printTree(ast.children[i], t+1);
  }
}

int main(int argc, char const *argv[]) {
  printf("# Grime Compiler\n");
  struct Token *tokens = lex(
    "func main(a: Int) -> Int {\n"
    "\treturn 19278 + 1\n"
    "}"
  );
  struct Node ast = parse(tokens);
  printTree(ast, 0);
  codegen(ast);
  free(tokens);
  return 0;
}
