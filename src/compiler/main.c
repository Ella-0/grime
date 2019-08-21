#include <stdio.h>
#include "node.h"
#include "parser.tab.h"
#include "codegen.h"

int main(int argc, char const *argv[]) {
	yyparse();
	//printNode(tld);
  	codegen(tld);
	return 0;
}
