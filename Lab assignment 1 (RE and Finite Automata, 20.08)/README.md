# Regular Expression to epsilon-NFA
 Code to generate epsilon-NFA for a given Regular Expression
 # Sample input: (a((ab|c)*a)bc)|c
 
- Size of my alphabet is 3. You can increase the size of the alphabet by changing the "#define" statement
- The precedence is as follows : brackets >> * >> concatenation >> |
- The function removebrackets simplifies the inner expression into a graph and returns it.
- Therefore to simplify the final step. I have wrapped the regex with a final pair of bracket eg: ((a((ab|c)*a)bc)|c)
- Code only checks if there are matching brackets or not.
- It does not check if the * or | are in wrong positions. 
- Output is a transition table: list of nodes and new node upon a step by consuming a symbol.
