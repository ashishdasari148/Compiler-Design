%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lex.yy.c"

void yyerror(char* s);
extern FILE *yyin;
extern int yylineno;
int k=1;
%}
%union {
    char var[10];
}
%token DIV LEFT_PAREN MINUS MULT PLUS RIGHT_PAREN 
%token <var> NUMBER
%type <var> answer exp expa expm expd exps

%start answer

%%
answer : exp                        {
										
										printf("Answer = %s\n",$1);
									}
exp : exp MINUS exps                {
										char temp[5];
										$$[0]='T';$$[1]='\0';
										sprintf(temp,"%d",k);k++;
										strcat($$,temp);
										printf("%s = %s - %s\n",$$,$1,$3);
									}
    | exps                          
    ;
exps : exps PLUS expa               {
										char temp[5];
										$$[0]='T';$$[1]='\0';
										sprintf(temp,"%d",k);k++;
										strcat($$,temp);
										printf("%s = %s + %s\n",$$,$1,$3);
									}
    | expa                          
    ;
expa : expa MULT expm               {
										char temp[5];
										$$[0]='T';$$[1]='\0';
										sprintf(temp,"%d",k);k++;
										strcat($$,temp);
										printf("%s = %s * %s\n",$$,$1,$3);
									}
    | expm                          
    ;
expm : expm DIV expd                {
										char temp[5];
										$$[0]='T';$$[1]='\0';
										sprintf(temp,"%d",k);k++;
										strcat($$,temp);
										printf("%s = %s / %s\n",$$,$1,$3);
									}
    | expd                          
    ;
expd : LEFT_PAREN exp RIGHT_PAREN   {strcpy($$,$2);}
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
