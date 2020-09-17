%{
    #define NUMBER 1
    #define RELOP 2
    #define IF 3
    #define THEN 4
    #define ELSE 5
    #define ID 6
%}
whitespace ([ \t\n]*)
number ([+-]?[0-9]*([.][0-9]*)?(E[+-]?[0-9]*)?)
relop (<>|<=|>=|<|>|=)
if (if)
then (then)
else (else)
id ([A-Za-z][A-Za-z0-9]*)

%%
{number} {return NUMBER;}
{relop} {return RELOP;}
{if} {return IF;}
{then} {return THEN;}
{else} {return ELSE;}
{id} {return ID;}
{whitespace} {}
. {return -1;}
%%

int yywrap(void) {return 1;}
int main() {
    int token;
    yyin=fopen("myfileQ2.txt","r");
    while ((token=yylex())!=0)
        printf("%d %s \n", token, yytext);
}