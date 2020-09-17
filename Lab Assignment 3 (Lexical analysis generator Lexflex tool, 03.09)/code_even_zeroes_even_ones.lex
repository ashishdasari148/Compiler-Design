%{

%}
even_zeroes_even_ones ((00|11)*((01|10)(00|11)*(01|10)(00|11)*)*)

%%
{even_zeroes_even_ones} {printf("YES\n");}
.* {printf("NO\n");}
%%

int yywrap(void) {return 1;}
int main() {
    yylex();
    return 0;
}