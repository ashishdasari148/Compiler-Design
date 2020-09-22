%{
    #define PLUS 19
    #define MINUS 20
    #define PROD 21
    #define DIV 22
    #define RELOPEQUAL 23
    #define LESSTHAN 24
    #define GREATERTHAN 25
    #define BRACKETOPEN 26
    #define BRACKETCLOSE 27
    #define CURLYBRACKETOPEN 28
    #define CURLYBRACKETCLOSE 29
    #define ASSIGNEQUAL 30
    #define SEMICOLON 31
    #define AND 32
    #define ELSE 33
    #define END 34
    #define INT_C 35
    #define ID 36
    #define IF 37
    #define INT_ 38
    #define DO 39
    #define FLOAT_C 40
    #define FLOAT_ 41
    #define NOT 42
    #define OR 43
    #define PRINT 44
    #define PROG 45
    #define SCAN 46
    #define STR 47
    #define THEN 48
    #define WHILE 49
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
