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
{number} {yylval.flt = (float)atof(yytext); return NUMBER;}
{whitespace} {}
%%