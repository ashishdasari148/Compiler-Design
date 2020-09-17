%{

%}
div_by_3 (0*|(0*(1(01*0)*10*)+))

%%
{div_by_3} {printf("YES\n");}
.* {printf("NO\n");}
%%

int yywrap(void) {return 1;}
int main() {
    yylex();
    return 0;
}