%{
#include <stdio.h> 
int yylex();
void yyerror();
%}

%token ZERO ONE
%start input

%%

input : { printf("Start input:\n"); } | input line ;

line : ones zeros ones zeros '\n' { if ($1 == $4 && $2 == $3) { printf("Correct\n");} else { yyerror(); }} | '\n' { printf("Empty line\n"); } | error'\n' { yyerrok; } ;

zeros:ZERO{++$$;} | zeros ZERO {++$$;}; 

ones:ONE{++$$;} | ones ONE {++$$;}; 

%%

int yylex() {
char c;
c = getchar(); 
switch(c) {
case '0': return(ZERO); 
case '1': return(ONE); 
case EOF: return(0); 
default: return(c);
} 
}

void yyerror() { printf("Incorrect\n"); } 
int main() { return(yyparse()); }