Please chance the current working directory to where the fires have been downloaded.
To run the code:
>> cd <directory of the files>
>> flex <FILE NAME>
>> gcc lex.yy.c
>> ./a.out

#Q1---------------------------------------------------------------------------------
------------------------------------------------------------------------------------
--PART-1----------------------------------------------------------------------------
FILE NAME: code_even_zeroes.lex

Regular expression used: ((1|01*01*)*)

Sample inputs:
0000100101010110100001
0000100100010110100001

--PART-2----------------------------------------------------------------------------
FILE NAME: code_even_zeroes_even_ones.lex

Regular expression used: ((00|11)*((01|10)(00|11)*(01|10)(00|11)*)*)

Sample inputs:
0000100101010110100001
0000100100010110100001

--PART-3----------------------------------------------------------------------------
FILE NAME: code_div_by_3.lex

Regular expression used: (0*|(0*(1(01*0)*10*)+))

Sample inputs:
0
11
1001
01001
00

#Q2---------------------------------------------------------------------------------
------------------------------------------------------------------------------------
FILE NAME: codeQ2.lex

Regular Expressions used:
whitespace ([ \t\n]*)
number ([+-]?[0-9]*([.][0-9]*)?(E[+-]?[0-9]*)?)
relop (<>|<=|>=|<|>|=)
if (if)
then (then)
else (else)
id ([A-Za-z][A-Za-z0-9]*)

Sample input is given as a text file named <myfileQ2.txt>

#Q3---------------------------------------------------------------------------------
------------------------------------------------------------------------------------
FILE NAME: codeQ3.lex

Regular Expressions used:
whitespace ([ \t\n]*)
main [main]
number ([+-]?[0-9]*([.][0-9]*)?(E[+-]?[0-9]*)?)
operator [*/%+-]
assignment (=|[+][=]|[-][=]|[*][=]|[/][=]|[%]=)
unaryop ([+][+]|[-][-])
relop (!=|<=|>=|<|>|==)
if [if]
else [else]
for [for]
while [while]
read [read]
print [print]
int (int|INT)
float (float|FLOAT)
separator [;,(){}]
id ([A-Za-z][A-Za-z0-9]*)

Sample input is given as a text file named <myfileQ3.txt>