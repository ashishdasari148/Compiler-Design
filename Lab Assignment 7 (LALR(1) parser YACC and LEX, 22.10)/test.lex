%{
#include <stdio.h>
#include "y.tab.h"
%}

%%
ding {return DING;}
dong {return DONG;}
dell {return DELL;}
[ ]* ;
\n|. return yytext[0];
%%
