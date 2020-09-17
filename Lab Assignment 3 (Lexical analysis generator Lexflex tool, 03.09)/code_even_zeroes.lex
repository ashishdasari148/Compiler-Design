%{

%}
even_zeroes ((1|01*01*)*)

%%
{even_zeroes} {printf("YES\n");}
.* {printf("NO\n");}
%%

int yywrap(void) {return 1;}
int main() {
    yylex();
    return 0;
}