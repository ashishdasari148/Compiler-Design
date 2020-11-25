%{
#include <stdio.h>
#include "y.tab.h"
#include <stdlib.h>
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
{number} {strcpy(yylval.var, yytext); return NUMBER;}
{whitespace} {}
%%
