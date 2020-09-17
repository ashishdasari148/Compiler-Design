# custom input: if input<>+10 then output1=100 else elsenotakeyword output2>=100
import re
if __name__ == "__main__":
    x = input("Enter the input: ")
    output = []
    check = [["^\s*", ""], ["[+-]?\d*([.]\d*)?(E[+-]?\d*)?", "number"], ["<>|<=|>=|<|>|=", "relop"], ["if", "if"], ["then","then"], ["else","else"], ["[A-Za-z][A-Za-z0-9]*","id"]]
    while(len(x)):
        maxmatch = 0
        match = (re.match("", x), "")
        for i,j in check:
            temp = re.match(i, x)
            if (temp):
                if(temp.span()[1]-temp.span()[0] > maxmatch):
                    match = (temp, j)
                    maxmatch = temp.span()[1]-temp.span()[0]
        token, tokentype = match
        if(len(tokentype)):
            print("(" + str(tokentype) + "," + str(token.group()) + ")")
        x = x.replace(token.group(), '', 1)
        