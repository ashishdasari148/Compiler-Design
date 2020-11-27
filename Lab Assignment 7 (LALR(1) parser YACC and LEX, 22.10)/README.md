# LALR(1) Predictive Parser using yacc tool
- Note: --report=all --report-file=logfile arguments produce a report mentioning all the states, items and transitions.

## Test Grammar:

```bash
flex test.lex
yacc -d test.y
gcc y.tab.c
```
 
Inputs: 

```bash
./a.out
ding dong dell

./a.out
ding dong bell

./a.out
ding dong dell dell
```
## Large Grammar:

```bash
flex grammar.lex
yacc -d grammar.y --report=all --report-file=logfile
gcc y.tab.c
```
 
Inputs: 

```bash
./a.out input1.txt              # valid syntax
./a.out input2.txt              # syntax error
./a.out input2_corrected.txt    # valid syntax
./a.out input3.txt              # syntax error
./a.out input3_corrected.txt    # valid syntax
```