%START AA EE II OO UU YY CONS ERROR

%%
<CONS,AA,EE,II,OO,UU,YY>\n {printf("Correct\n"); BEGIN 0;};

<AA>[aAb-df-hj-np-tv-xzB-DF-HJ-NP-TV-XZ]+ ;
<EE>[eEb-df-hj-np-tv-xzB-DF-HJ-NP-TV-XZ]+ ;
<II>[iIb-df-hj-np-tv-xzB-DF-HJ-NP-TV-XZ]+ ;
<OO>[oOb-df-hj-np-tv-xzB-DF-HJ-NP-TV-XZ]+ ;
<UU>[uUb-df-hj-np-tv-xzB-DF-HJ-NP-TV-XZ]+ ;
<YY>[yYb-df-hj-np-tv-xzB-DF-HJ-NP-TV-XZ]+ ;
<CONS>[b-df-hj-np-tv-xzB-DF-HJ-NP-TV-XZ]+ ;

<CONS>[aA] BEGIN AA;
<CONS,AA>[eE] BEGIN EE;
<CONS,AA,EE>[iI] BEGIN II;
<CONS,AA,EE,II>[oO] BEGIN OO;
<CONS,AA,EE,II,OO>[uU] BEGIN UU;
<CONS,AA,EE,II,OO,UU>[yY] BEGIN YY;

<CONS>\n {printf("Error\n"); BEGIN 0;};
<CONS,AA,EE,II,OO,UU,YY>. BEGIN ERROR;
<ERROR>.*\n {printf("Error\n"); BEGIN 0;};

[b-df-hj-np-tv-xzB-DF-HJ-NP-TV-XZ] BEGIN CONS;
[aA] BEGIN AA;
[eE] BEGIN EE;
[iI] BEGIN II;
[oO] BEGIN OO;
[uU] BEGIN UU;
[yY] BEGIN YY;

\n {BEGIN 0; printf("Empty line\n");};
. BEGIN ERROR;

%%
int yywrap(){return 1;}
int main(){printf("Enter the line:\n"); yylex();}



