%{
    #define PROD 4
    #define RELOPEQUAL 5
    #define ID 6
%}
whitespace ([ \t\n]*)
prod ([*])
relopequal ([=])
id ([A-Za-z][A-Za-z0-9]*)
%%
{prod} {return PROD;}
{relopequal} {return RELOPEQUAL;}
{id} {return ID;}
{whitespace} {}
. {return -1;}
%%
int yywrap(void) {return 1;}
