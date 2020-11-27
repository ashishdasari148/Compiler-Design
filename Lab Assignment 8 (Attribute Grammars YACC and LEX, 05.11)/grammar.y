%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lex.yy.c"

/*typedef union {
float flt;
} YYSTYPE;*/

void yyerror(char* s);
extern FILE *yyin;
extern int yylineno;
%}
%union {
    float flt; 
}
%token DIV LEFT_PAREN MINUS MULT PLUS RIGHT_PAREN 
%token <flt> NUMBER
%type <flt> answer exp expa expm expd exps

%start answer

%%
answer : exp                        {printf("Answer according to BODMAS rule = %f\n", $1);}
exp : exp MINUS exps                {$$ = $1 - $3;printf("%f - %f = %f\n", $1, $3, $1 - $3);}
    | exps                          
    ;
exps : exps PLUS expa               {$$ = $1 + $3;printf("%f + %f = %f\n", $1, $3, $1 + $3);}
    | expa                          
    ;
expa : expa MULT expm               {$$ = $1 * $3;printf("%f * %f = %f\n", $1, $3, $1 * $3);}
    | expm                          
    ;
expm : expm DIV expd                {$$ = $1 / (float)$3;printf("%f / %f = %f\n", $1, $3, $1 / (float)$3);}
    | expd                          
    ;
expd : LEFT_PAREN exp RIGHT_PAREN   {$$ = $2;}
    | NUMBER                        
    ;
%%	
void yyerror(char *s){	
	printf("Error: %s\n",s);	
	exit(-1);	
}
int yywrap(){return 1;}
int main(int argc, char** argv){
    yyin=fopen(argv[1],"r");
    /*int x;
    while(x = yylex())
        printf("%d\t%s\n", x, yytext);*/
    return yyparse();
}