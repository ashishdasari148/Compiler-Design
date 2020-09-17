%{
    #define MAIN 1
    #define NUMBER 2
    #define RELOP 3
    #define IF 4
    #define ELSE 5
    #define FOR 6
    #define WHILE 7
    #define ID 12
    #define READ 13
    #define PRINT 14
    #define INT_ 15
    #define FLOAT_ 16
    #define UNARYOP 17
    #define ASSIGNMENT 18
    #define OPERATOR 19
    #define SEPARATOR 20
%}
whitespace ([ \t\n]*)
main (main)
number ([+-]?[0-9]*([.][0-9]*)?(E[+-]?[0-9]*)?)
operator [*/%+-]
assignment (=|[+][=]|[-][=]|[*][=]|[/][=]|[%]=)
unaryop ([+][+]|[-][-])
relop (!=|<=|>=|<|>|==)
if (if)
else (else)
for (for)
while (while)
read (read)
print (print)
int (int|INT)
float (float|FLOAT)
separator [;,(){}]
id ([A-Za-z][A-Za-z0-9]*)

%%
{main} {return MAIN;}
{number} {return NUMBER;}
{operator} {return OPERATOR;}
{assignment} {return ASSIGNMENT;}
{unaryop} {return UNARYOP;}
{relop} {return RELOP;}
{if} {return IF;}
{else} {return ELSE;}
{for} {return FOR;}
{while} {return WHILE;}
{read} {return READ;}
{print} {return PRINT;}
{int} {return INT_;}
{float} {return FLOAT_;}
{separator} {return SEPARATOR;}
{id} {return ID;}
{whitespace} {}
. {return -1;}
%%

int yywrap(void) {return 1;}
int main() {
    int token;
    yyin=fopen("myfileQ3.txt","r");
    while ((token=yylex())!=0)
        printf("%2d  %s \n", token, yytext);
}