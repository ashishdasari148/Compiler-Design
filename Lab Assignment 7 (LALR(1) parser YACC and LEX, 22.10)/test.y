%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lex.yy.c"
void yyerror(char* s);
%}

%token DING DONG DELL
%start rhyme
%%
rhyme : sound place '\n' {printf("string valid\n"); exit(0);};
sound : DING DONG ;
place : DELL ;
%%

int yywrap(){return 1;}
void yyerror(char* s){printf("%s\n", s);}
int main(){return yyparse();}