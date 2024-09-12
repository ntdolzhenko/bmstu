%{

#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

%}

DIGIT [1-9][0-9]*
SIGN (>|<|>=|<=|!=)
VAR [A-Za-z]
LBRACE [(]
RBRACE [)]
PN_TOKEN [+-]

%%

{DIGIT} {yylval.digit = atoi(yytext); return DIGIT;}
{VAR} {yylval.var_ = *yytext; return VAR;} 
{SIGN} {return SIGN;} 
{PN_TOKEN} {yylval.pn_ = *yytext; return PN_TOKEN;} 
{LBRACE} {yylval.lbrace = *yytext; return LBRACE;}
{RBRACE} {yylval.rbrace = *yytext; return RBRACE;}

\n {return '\n';};
. {};

%%

int yywrap() {return 1;}
