%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lex.yy.c"

void print_tabs(int depth){
    while(depth--)
        printf("\t");
    return;
}

void print_tree_preorder(node * root, int depth)
{
    if(root==NULL){
        print_tabs(depth);
        printf("NULL\n");
		return;
    }
    print_tabs(depth);
	if(root->is_char==1)
		printf("Value: %c;\n",(char)(int)root->value);
	else
		printf("Value: %f;\n",root->value);
    print_tabs(depth);
	printf("LEFT: {\n");
	print_tree_preorder(root->left, depth+1);
    print_tabs(depth);
    printf("};\n");
    print_tabs(depth);
    printf("RIGHT: {\n");
	print_tree_preorder(root->right, depth+1);
    print_tabs(depth);
    printf("}\n");
}

void yyerror(char* s);
extern FILE *yyin;
extern int yylineno;
%}

%code requires {
    typedef struct node{
        int is_char;
        float value;
        struct node *left;
        struct node *right;
    }node;
}

%union {
    node* node_ptr; 
}

%token DIV LEFT_PAREN MINUS MULT PLUS RIGHT_PAREN 
%token <node_ptr> NUMBER
%type <node_ptr> answer exp expa expm expd exps

%start answer

%%
answer : exp                        {printf("Result printed inorder:\n");printf("{\n");print_tree_preorder($1, 1);printf("}\n");return 0;}
exp : exp MINUS exps                { node *nn= (node *)malloc(sizeof(node));
                                    nn->is_char=1;
                                    nn->value=(int)'-';
                                    nn->left=$1;
                                    nn->right=$3;
                                    $$=nn;
                                    }
    | exps                          
    ;
exps : exps PLUS expa               { node *nn= (node *)malloc(sizeof(node));
                                    nn->is_char=1;
                                    nn->value=(int)'+';
                                    nn->left=$1;
                                    nn->right=$3;
                                    $$=nn;
                                    }
    | expa                          
    ;
expa : expa MULT expm               { node *nn= (node *)malloc(sizeof(node));
                                    nn->is_char=1;
                                    nn->value=(int)'*';
                                    nn->left=$1;
                                    nn->right=$3;
                                    $$=nn;
                                    }
    | expm                          
    ;
expm : expm DIV expd                { node *nn= (node *)malloc(sizeof(node));
                                    nn->is_char=1;
                                    nn->value=(int)'/';
                                    nn->left=$1;
                                    nn->right=$3;
                                    $$=nn;
                                    }
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