// Definitions

%{

#include <stdio.h>
int yylex();
void yyerror(char const *);

%}

token NAME;

%%

// Rules

input: '{' levels '}';

levels: level | level ',' levels;

level: '{' ranks '}';

ranks: rank | rank ',' ranks;

rank: '{' names '}';

names: NAME | NAME ',' names;

%%

void yyerror(char const *s) {
  fprintf(stderr, "%s\n", s);
}

int main(int argc, char *const argv[]) {
  
}

// Functions
