%{
  #include <stdio.h>
  void yyerror(char *s);
  int yylex();
  #include "node.h"
%}

%union {
  struct Node node;
  int token;
  char *string;
}
%start tld
%token END 0
%token TFUNC "func" TRETURN "return" TWHILE "while" TIF "if" TVAR "var" TVAL "val"
%token TADD "+" TSUB "-" TMUL "*" TDIV "/"
%token TLPA "(" TRPA ")" TLBR "{" TRBR "}" TLBA "[" TRBA "]"
%token TARR "->" TCLN ":" TSMI
%token TCOM ","
%token TID TINT
%type <string> TID
%type <node> identifier functions function params type vardecl valdecl
%type <node> arraytype simpletype param ifexpr whileexpr expr blk blkparts operatorexpr
%right "if" "while"
%right "=" "val" "var"
%left "+"
%left "*"
%left TINT
%%

identifier: TID {$$ = createNode($1, 0, NULL);}
  ;

tld: functions {tld = createNode("NTLD", 1, (struct Node []) {$1});}
  ;

functions: function {$$ = createNode("NFUNCTIONS", 1, (struct Node []) {$1});}
  ;

function: "func" identifier "(" params ")" "->" type expr {$$ = createNode("NFUNCTION", 4, (struct Node []) {$2, $4, $7, $8});}
  ;

params: param "," params {$$ = createNode("NPARAMS", 2, (struct Node []) {$1, $3});}
  | param {$$ = createNode("NPARAMS", 1, (struct Node []) {$1});}
  ;

param: identifier ":" type {$$ = createNode("NPARAM", 2, (struct Node []) {$1, $3});}
  ;

arraytype: "[" type "]" {$$ = createNode("NARRAYTYPE", 1, (struct Node []) {$2});}
  ;

simpletype: identifier {$$ = createNode("NSIMPLETYPE", 1, (struct Node []) {$1});}
  ;

type: arraytype {$$ = createNode("NTYPE", 1, (struct Node []) {$1});}
  | simpletype {$$ = createNode("NTYPE", 1, (struct Node []) {$1});}
  ;

blk: "{" blkparts "}" {$$ = createNode("NBLK", 1, (struct Node []) {$2});}
  ;

blkparts: blkparts expr {$$ = createNode("NBLKPARTS", 2, (struct Node []) {$1, $2});}
  | %empty {$$ = createNode("BLKPARTS", 0, NULL);}
  ;

ifexpr: "if" "(" expr ")" expr {$$ = createNode("NIFEXPR", 2, (struct Node []) {$3, $5});} %prec "if"
  ;

whileexpr: "while" "(" expr ")" expr {$$ = createNode("NWHILEEXPR", 2, (struct Node []) {$3, $5});} %prec "while"
  ;

vardecl: "var" identifier ":" type "=" expr {$$ = createNode("NVAREXPR", 3, (struct Node []) {$2, $4, $6});} %prec "var"
  ;
valdecl: "val" identifier ":" type "=" expr {$$ = createNode("NVALEXPR", 2, (struct Node []) {$2, $4, $6});} %prec "val"
  ;

operatorexpr: TINT {$$ = createNode("NINTEGER", 0, NULL);} %prec "*"
  | expr "+" expr {$$ = createNode("NADDEXPR", 2, (struct Node []) {$1, $3});} %prec "+"
  | expr "*" expr {$$ = createNode("NMULEXPR", 2, (struct Node []) {$1, $3});} %prec "*"
  ;

expr: blk {$$ = createNode("NEXPR", 1, (struct Node []) {$1});}
  | ifexpr {$$ = createNode("NEXPR", 1, (struct Node []) {$1});}
  | whileexpr {$$ = createNode("NEXPR", 0, NULL);}
  | vardecl {$$ = createNode("NEXPR", 0, NULL);}
  | valdecl {$$ = createNode("NEXPR", 0, NULL);}
  | operatorexpr {$$ = createNode("NEXPR", 1, (struct Node []) {$1});}
  ;
%%

void yyerror(char *s) {printf("error : %s\n", s);}
