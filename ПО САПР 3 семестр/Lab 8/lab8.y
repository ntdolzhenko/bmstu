%{
#include<stdio.h>
int yylex();
int main();
void yyerror();
%}

%start input

%%

input: input '\n' {printf("Correct\n");}
	|input aa '\n' {printf("Correct\n");};
	|input ee '\n' {printf("Correct\n");}
	|input ii '\n' {printf("Correct\n");}
	|input oo '\n' {printf("Correct\n");}
	|input uu '\n' {printf("Correct\n");}
	|input yy '\n' {printf("Correct\n");}
	|'\n' { printf("Empty line\n"); }
	|error '\n' { yyerrok; };

aa: 'a' | 'A' | aa 'a'| aa 'A'| aa cons | cons aa;

ee: 'e' | 'E' | ee 'e'| ee 'E'| ee cons | cons ee | aa ee;
ii: 'i' | 'I' | ii 'i'| ii 'I'| ii cons | cons ii | aa ii | ee ii;
oo: 'o' | 'O' | oo 'o'| oo 'O'| oo cons | cons oo | aa oo | ee oo | ii oo;
uu: 'u' | 'U' | uu 'u'| uu 'U'| uu cons | cons uu | aa uu | ee uu | ii uu | oo uu;
yy: 'y' | 'Y' | yy 'y'| yy 'Y'| yy cons | cons yy | aa yy | ee yy | ii yy | oo yy | uu yy;


cons: 'b'|'c'|'d'|'f'|'g'|'h'|'j'|'k'|'l'|'m'
	|'n'|'p'|'q'|'r'|'s'|'t'|'v'|'w'|'x'|'z'
	|'B'|'C'|'D'|'F'|'G'|'H'|'J'|'K'|'L'|'M'
	|'N'|'P'|'Q'|'R'|'S'|'T'|'V'|'W'|'X'|'Z'
	|cons 'b'|cons 'c'|cons 'd'|cons 'f'|cons 'g'
	|cons 'h'|cons 'j'|cons 'k'|cons 'l'|cons 'm'
	|cons 'n'|cons 'p'|cons 'q'|cons 'r'|cons 's'
	|cons 't'|cons 'v'|cons 'w'|cons 'x'|cons 'z'
	|cons 'B'|cons 'C'|cons 'D'|cons 'F'|cons 'G'
	|cons 'H'|cons 'J'|cons 'K'|cons 'L'|cons 'M'
	|cons 'N'|cons 'P'|cons 'Q'|cons 'R'|cons 'S'
	|cons 'T'|cons 'V'|cons 'W'|cons 'X'|cons 'Z';


%%

int yylex(){return(getchar());}

void yyerror() { printf("Incorrect\n");} 

int main(){printf("Enter the line:\n"); return (yyparse());}


