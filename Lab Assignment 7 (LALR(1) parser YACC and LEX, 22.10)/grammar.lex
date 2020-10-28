%{
#include <stdio.h>
#include "y.tab.h"
%}
int_const ([0-9]+)
float_const ([0-9]*([.][0-9]+)?(E[+-]?[0-9]*)?)
id ([A-Za-z][A-Za-z0-9_]*)
str (\".*\")
whitespace ([ \n\t]*)
%%
[:][=] {return ASSIGN;}
[:] {return COLON;}
[;] {return SEMICOLON;}
[,] {return COMMA;}
[+] {return PLUS;}
[-] {return MINUS;}
[*] {return MULT;}
[/] {return DIV;}
[.] {return DOT;}
[=] {return EQ;}
[<] {return LT;}
[>] {return GT;}
[>][=] {return GE;}
[<][=] {return LE;}
[<][>] {return NE;}
[-][>] {return RETURNS;}
[(] {return LEFT_PAREN;}
[)] {return RIGHT_PAREN;}
[[] {return LEFT_SQ_BKT;}
[]] {return RIGHT_SQ_BKT;}
and {return AND;}
def {return DEF;}
else {return ELSE;}
end {return END;}
exit {return EXITLOOP;}
int {return INT;}
float {return FLOAT;}
from {return FROM;}
fun {return FUN;}
global {return GLOBAL;}
if {return IF;}
mod {return MOD;}
not {return NOT;}
null {return NUL;}
or {return OR;}
print {return PRINT;}
product {return PRODUCT;}
read {return READ;}
return {return RETURN;}
skip {return SKIP;}
step {return STEP;}
to {return TO;}
while {return WHILE;}
{int_const} {return INT_CONST;}
{float_const} {return FLOAT_CONST;}
{id} {return ID;}
{str} {return STRING;}
[%][dsf] {return FORMAT;}
([/][/].*[\n]) {}
{whitespace} {}
%%