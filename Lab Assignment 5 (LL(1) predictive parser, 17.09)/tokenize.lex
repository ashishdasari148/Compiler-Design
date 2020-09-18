%{
    #define PLUS 1
    #define MINUS 2
    #define PROD 3
    #define DIV 4
    #define RELOPEQUAL 5
    #define LESSTHAN 6
    #define GREATERTHAN 7
    #define BRACKETOPEN 12
    #define BRACKETCLOSE 13
    #define CURLYBRACKETOPEN 14
    #define CURLYBRACKETCLOSE 5
    #define ASSIGNEQUAL 6
    #define SEMICOLON 7
    #define AND 12
    #define ELSE 14
    #define END 5
    #define INT_C 6
    #define ID 7
    #define IF 12
    #define INT_ 13
    #define DO 14
    #define FLOAT_C 15
    #define FLOAT_ 16
    #define NOT 17
    #define OR 18
    #define PRINT 19
    #define PROG 20
    #define SCAN 17
    #define STR 18
    #define THEN 19
    #define WHILE 20
%}
whitespace ([ \t\n]*)
plus ([+])
minus ([-])
prod ([*])
div ([/])
relopequal ([=])
lessthen ([<])
greaterthan ([>])
bracketopen ([(])
bracketclose ([)])
curlybracketopen ([{])
curlybracketclose ([}])
assignequal (:=)
semicolon ([;])
and (and)
else (else)
end (end)
int_c ([+-]?[0-9]*)
id ([A-Za-z][A-Za-z0-9]*)
if (if)
int_ (int)
do (do)
float_c ([+-]?[0-9]*([.][0-9]*)?(E[+-]?[0-9]*)?)
float_ (float)
not (not)
or (or)
print (print)
prog (prog)
scan (scan)
str ("[A-Za-z0-9!#%&()*+,-./:;<=>?[]^_{|}~ ]*")
then (then)
while (while)
%%
{plus} {return PLUS;}
{minus} {return MINUS;}
{prod} {return PROD;}
{div} {return DIV;}
{relopequal} {return RELOPEQUAL;}
{lessthen} {return LESSTHAN;}
{greaterthan} {return GREATERTHAN;}
{bracketopen} {return BRACKETOPEN;}
{bracketclose} {return BRACKETCLOSE;}
{curlybracketopen} {return CURLYBRACKETOPEN;}
{curlybracketclose} {return CURLYBRACKETCLOSE;}
{assignequal} {return ASSIGNEQUAL;}
{semicolon} {return SEMICOLON;}
{and} {return AND;}
{else} {return ELSE;}
{end} {return END;}
{int_c} {return INT_C;}
{if} {return IF;}
{int_} {return INT_;}
{do} {return DO;}
{float_c} {return FLOAT_C;}
{float_} {return FLOAT_;}
{not} {return NOT;}
{or} {return OR;}
{print} {return PRINT;}
{prog} {return PROG;}
{scan} {return SCAN;}
{then} {return THEN;}
{while} {return WHILE;}
{id} {return ID;}
{str} {return STR;}
{whitespace} {}
. {return -1;}
%%
int yywrap(void) {return 1;}
