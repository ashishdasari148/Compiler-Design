%{
    #define PLUS 3
    #define PROD 4
    #define BRACKETOPEN 6
    #define BRACKETCLOSE 7
    #define ID 5
%}
whitespace ([ \t\n]*)
plus ([+])
prod ([*])
bracketopen ([(])
bracketclose ([)])
id ([A-Za-z][A-Za-z0-9]*)
%%
{plus} {return PLUS;}
{prod} {return PROD;}
{bracketopen} {return BRACKETOPEN;}
{bracketclose} {return BRACKETCLOSE;}
{id} {return ID;}
{whitespace} {}
. {return -1;}
%%
int yywrap(void) {return 1;}
