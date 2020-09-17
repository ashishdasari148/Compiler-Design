The file "code.cpp" contains code that takes a grammar G and:
Removes left-recursion from grammar G
Produces an equivalent left-factored grammar
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
(next is optional)
next line: <number_of_tokens> <start symbol>
<space separated number_of_tokens tokens>
-------------------------------------------------------------------------------------------------------------
SAMPLE INPUT 1 (with sequence of tokens that doesn't belong to grammar)--------------------------------------
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

13 E
id + ( id * ( id + id ) + id +

-------------------------------------------------------------------------------------------------------------
SAMPLE INPUT 2 (with sequence of tokens that belongs to grammar)---------------------------------------------
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

13 E
id + ( id * ( id + id ) ) + id

-------------------------------------------------------------------------------------------------------------
SAMPLE INPUT 3 (grammar with empty productions)--------------------------------------------------------------
4
A 0
a 1
B 0
c 1
1
A
3
3
a A B
3
a A c
3
a B c