%{
#include <stdio.h>
#include "y.tab.h"
%}
number ([0-9]*([.][0-9]+)?(E[+-]?[0-9]*)?)
whitespace ([ \n\t]*)
%%
[+] {return PLUS;}
[-] {return MINUS;}
[*] {return MULT;}
[/] {return DIV;}
[(] {return LEFT_PAREN;}
[)] {return RIGHT_PAREN;}
{number} {node *n=(node *)malloc(sizeof(node));
		n->is_char=0;
		n->left=NULL;
		n->right=NULL;
		n->value=(float)atof(yytext);
        yylval.node_ptr = n;
        return NUMBER;
        }
{whitespace} {}
%%