%{
#include <stdio.h>
int yylex(); void yyerror(); %}
%token ONE
%start input

%%

input : { printf("Start input:\n"); } | input correct ;
correct : even '\n' { printf("Correct\n"); } | error '\n' { yyerrok; } | '\n' { printf("Empty line\n"); } ;
even : odd ONE ;
odd:ONE | even ONE; 

%%

int yylex() {

char c;
c = getchar(); 
switch(c) {
case '1': return (ONE);
case EOF: return 0; 
default: return c;
} 
}

void yyerror() { printf("Incorrect\n"); } 
int main() { return(yyparse()); }