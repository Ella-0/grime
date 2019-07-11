%{

  #include "node.h"
  #include "parser.tab.h"

%}

%%

[ \n\t] ;
"func" {return TFUNC;};
"var" {return TVAR;};
"val" {return TVAL;};
"if" {return TIF;};
"while" {return TWHILE;};
"return" {return TRETURN;};
"->" {return TARR;};
"{" {return TLBR;};
"}" {return TRBR;};
"(" {return TLPA;};
")" {return TRPA;};
"[" {return TLBA;};
"]" {return TRBA;};
":" {return TCLN;};
"+" {return TADD;};
"," {return TCOM;};
"=" {return TASS;};
[+-]?[0-9]+ {yylval.string = malloc(yyleng * sizeof(char)); strcpy(yylval.string, yytext); return TINT;};
[a-zA-Z]+ {yylval.string = malloc(yyleng * sizeof(char)); strcpy(yylval.string, yytext); return TID;};
%%

int yywrap() {return 1;}
