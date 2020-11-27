%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lex.yy.c"

#define MAXNODES 100
typedef struct node{
    float data;
    int left;
    int right;
    int is_char;
}node;
node node_data[MAXNODES]; /* data - left - right - ischar */
int node_count = 0;
int time_stamp = 1;
int addNode(float data, int left, int right, int isChar);
int nodeIsPresent(float data, int left, int right, int is_char);
void printDAG();
void justPrint(int prodNum, char x, int l, int r);

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
answer : exp                        {printf("Result Nodes of DAG:\n");printDAG();return 0;}
exp : exp MINUS exps                {$$ = addNode((float)'-', (int)$1, (int)$3, 1);}
    | exps                          
    ;
exps : exps PLUS expa               {$$ = addNode((float)'+', (int)$1, (int)$3, 1);}
    | expa                          
    ;
expa : expa MULT expm               {$$ = addNode((float)'*', (int)$1, (int)$3, 1);}
    | expm                          
    ;
expm : expm DIV expd                {$$ = addNode((float)'/', (int)$1, (int)$3, 1);}
    | expd                          
    ;
expd : LEFT_PAREN exp RIGHT_PAREN   {$$ = $2;}
    | NUMBER                        {$$ = addNode($1, 0, 0, 0);}
    ;
%%	
void yyerror(char *s){	
	printf("Error: %s\n",s);	
	exit(-1);	
}
int yywrap(){return 1;}
int nodeIsPresent(float data, int left, int right, int is_char) {
    for (int i = 1; i <= node_count; i++) {
        if ((node_data[i].data == data) && (node_data[i].left == left) && (node_data[i].right == right) && (node_data[i].is_char == is_char)) {
            return i;
        }
    }
    return 0;
}
int addNode(float data, int left, int right, int isChar) {
    int tempId = nodeIsPresent(data, left, right, isChar);
    if (tempId) {
        return tempId;
    }
    node_count++;
    node_data[node_count].data = data;
    node_data[node_count].left = left;
    node_data[node_count].right = right;
    node_data[node_count].is_char = isChar;
    return node_count;
}
void printDAG() {
    for (int i = 1; i <= node_count; i++) {
        printf("Node %d:\n", i);
        if (node_data[i].is_char) {
            printf("\tData: %c\n", (char)node_data[i].data);
        }
        else {
            printf("\tData: %f\n", node_data[i].data);
        }
        printf("\tLeft: %d\n\tRight: %d\n", node_data[i].left, node_data[i].right);
    }
}
int main(int argc, char** argv){
    yyin=fopen(argv[1],"r");
    /*int x;
    while(x = yylex())
        printf("%d\t%s\n", x, yytext);*/
    return yyparse();
}