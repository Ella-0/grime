%{

  #include "node.h"
  #include "parser.tab.h"

%}

%%

"func" {return TFUNC;};
"var" {return TVAR;};
"val" {return TVAL;};
"if" {return TIF;};
"while" {return TWHILE;};
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
[+-]?[0-9]+ {return TINT;};
[a-zA-Z]+ {return TID;};
[ \n\t] ;
%%

int yywrap() {return 1;}
