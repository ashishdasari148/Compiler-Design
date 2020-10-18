The file "code.cpp" contains code that takes a grammar G and:
Removes left-recursion from grammar G
Produces an equivalent left-factored grammar
Computes FIRST and FOLLOW sets of the symbols in the grammar
Takes a vector of tokens and gives the parse tree (if any)

Things to keep in mind:
While producing left-factored grammar, the new symbols generated are of format:
"<space><integer(same as new id generated)><space>"

-------------------------------------------------------------------------------------------------------------
Input Format-------------------------------------------------------------------------------------------------
line 1: <number_of_symbols>
    next number_of_symbols lines: <symbol> <1(if terminal)/0(if non-terminal)>
next line: <number_of_productions>
    next number_of_productions sections:
        <left symbol>
        <number_of_or_parts>
            next number_of_or_parts subsections: 
                <number_of_symbols_in_this_part>
                <space separated number_of_symbols_in_this_part symbols>
next line: <start symbol>

vector of tokens is taken from the file <inputfile.txt> (default: negative test case)
paste the contents of inputfileyes.txt in inputfile.txt for positive test case
paste the contents of inputfileno.txt in inputfile.txt for negative test case

-------------------------------------------------------------------------------------------------------------
SAMPLE INPUT 1 (to check the correctness of FIRST sets, FOLLOW sets and parse table)-------------------------
8
E 0
T 0
F 0
+ 1
* 1
id 1
( 1
) 1
3
E
2
3
E + T
1
T
T
2
3
T * F
1
F
F
2
3
( E )
1
id

P

-------------------------------------------------------------------------------------------------------------
SAMPLE INPUT 2 (large grammar, to check the Simulation of Non-recursive predictive parsing)------------------
50
AE 0
BE 0
D 0
DL 0
E 0
F 0
ES 0
IOS 0
IS 0
NE 0
P 0
PE 0
RE 0
S 0
SL 0
T 0
TY 0
VL 0
WS 0
+ 1
− 1
∗ 1
/ 1
= 1
< 1
> 1
( 1
) 1
{ 1
} 1
:= 1
; 1
and 1
else 1
end 1
ic 1
id 1
if 1
int 1
do 1
fc 1
float 1
not 1
or 1
print 1
prog 1
scan 1
str 1
then 1
while 1
19
P
1
4
prog DL SL end
DL
2
2
D DL
1
epsilon
D
1
3
TY VL ;
TY
2
1
int
1
float
VL
2
2
id VL
1
id
SL
2
2
S SL
1
epsilon
S
4
1
ES
1
IS
1
WS
1
IOS
ES
1
4
id := E ;
IS
2
5
if BE then SL end
7
if BE then SL else SL end
WS
1
5
while BE do SL end
IOS
2
2
print PE
2
scan id
PE
2
1
E
1
str
BE
2
3
BE or AE
1
AE
AE
2
3
AE and NE
1
NE
NE
3
2
not NE
3
{ BE }
1
RE
RE
3
3
E = E
3
E < E
3
E > E
E
3
3
E + T
3
E − T
1
T
T
3
3
T ∗ F
3
T / F
1
F
F
4
3
( E )
1
id
1
ic
1
fc

P
