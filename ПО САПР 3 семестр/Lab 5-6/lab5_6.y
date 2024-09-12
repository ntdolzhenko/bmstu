%{

#include <stdio.h>
#include <stdlib.h>
int yylex();
void yyerror();
%}

%token DIGIT VAR SIGN LBRACE RBRACE PN_TOKEN
%start input

%union{

int digit;
char var_;
char pn_;
char lbrace;
char rbrace;

}

%%
input : ; | input line ; 
line: exp sign exp '\n' {printf("\n");}
	| error '\n' {yyerrok;}
	|'\n' {printf("Empty line.\n");};


exp: l_brace exp r_brace
	| exp pn exp
	| pn num var
	| num var
	| num
	| var


l_brace: pn num LBRACE {printf("%c", $<lbrace>3);}
	| num LBRACE {printf("%c", $<lbrace>2);}
	| LBRACE {printf("%c", $<lbrace>1);};

r_brace: RBRACE {printf("%c", $<rbrace>1);};

pn: PN_TOKEN {printf("%c", $<pn_>1);};

num : DIGIT {printf("%d", $<digit>1);};
var: VAR DIGIT {printf("%c%d", $<var_>1, ++$<digit>2); };

sign: SIGN {printf("=");}


%%

void yyerror() { printf("Incorrect\n"); return; } 
int main() { yyparse(); }
