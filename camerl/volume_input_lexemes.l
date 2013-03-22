// Definitions

%{
  #include "y.tab.h"
%}

%%

// Rules

[A-Za-z_][A-Za-z0-9]* {
  yylval = yytext;
  return NAME;
}

[,{}]	return *yytext;;

[ \t\n] ;

%%

// Functions

int yywrap() {
  return 1;
}
