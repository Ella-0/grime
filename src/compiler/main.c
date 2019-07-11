#include <stdio.h>
#include "node.h"
#include "parser.tab.h"
#include "codegenerator.h"

int main(int argc, char const *argv[]) {
  yyparse();
  //printNode(tld);
  codegen(tld);
  return 0;
}
