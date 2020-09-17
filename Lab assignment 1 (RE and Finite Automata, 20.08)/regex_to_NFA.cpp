#include <bits/stdc++.h>
using namespace std;
# define noofsymbols 3
# define specialchar '$'
int nodenumber = 0;

typedef struct node{
    int id;
    vector<struct node *> to[noofsymbols+1];
}node;

typedef struct graph{
    node *enter;
    node *exit;
}graph;

typedef struct symbols{
    char x;
    graph *y;
}symbols;

vector<node *> final_table;

node *create_node(){
    node *x = new node;
    x->id = nodenumber;
    nodenumber++;
    final_table.push_back(x);
    return x;
}

graph *removestar(vector<symbols *> regex){
    node *u = create_node();
    node *v = create_node();
    regex[0]->y->exit->to[0].push_back(v);
    regex[0]->y->exit->to[0].push_back(regex[0]->y->enter);
    u->to[0].push_back(regex[0]->y->enter);
    u->to[0].push_back(v);
    graph *temp = new graph;
    temp->enter = u;
    temp->exit = v;
    return temp;
}

graph *concatenate(vector<symbols *>regex){
    graph *temp = new graph;
    for(int i=0;i<regex.size()-1;i++){
        regex[i]->y->exit->to[0].push_back(regex[i+1]->y->enter);
    }
    temp->enter = regex.front()->y->enter;
    temp->exit = regex.back()->y->exit;
    return temp;
}

graph *removebrackets(vector<symbols *> regex){
    vector<int> position_of_ors;
    for(int i=0;i<regex.size();i++){
        if(regex[i]->x == '*'){
            vector <symbols *> newregex(regex.begin()+i-1,regex.begin()+i);
            regex.erase(regex.begin()+i-1, regex.begin()+i+1);
            graph *temp = removestar(newregex);
            symbols *a = new symbols;
            a->x = specialchar;
            a->y = temp;
            regex.insert(regex.begin()+i-1, a);
            i = i-1;
        }
        if(regex[i]->x=='|')
            position_of_ors.push_back(i);
    }
    /*for(int i=0;i<regex.size();i++)
        cout << regex[i]->x;
    cout << " removed stars" << endl;*/
    vector <graph *> ready_to_or;
    while(position_of_ors.empty()==0){
        vector <symbols *> newregex(regex.begin()+position_of_ors.back()+1,regex.end());
        regex.erase(regex.begin()+position_of_ors.back(),regex.end());
        position_of_ors.pop_back();
        ready_to_or.push_back(concatenate(newregex));
    }
    ready_to_or.push_back(concatenate(regex));
    //cout << ready_to_or.size() << "no of graphs ready to or" << endl;
    if(ready_to_or.size()>1){
        node *u = create_node();
        node *v = create_node();
        graph *temp = new graph;
        temp->enter = u;
        temp->exit = v;
        for(int i=0; i<ready_to_or.size();i++){
            ready_to_or[i]->exit->to[0].push_back(v);
            u->to[0].push_back(ready_to_or[i]->enter);
        }
        return temp;
    }
    else
    {
        return ready_to_or[0];
    }
    
}

int main(){
    string input;
    cin >> input;
    //input = "(a((ab|c)*a)bc)|c";
    vector <symbols *> regex;
    symbols *a = new symbols;
    a->x = '(';
    a->y = NULL;
    regex.push_back(a);
    for(int i=0; i<input.length();i++){
        a = new symbols;
        a->x = input[i];
        if(!(input[i]=='('||input[i]==')'||input[i]=='|'||input[i]=='*')){
            node *u = create_node();
            node *v = create_node();
            u->to[input[i]-'a'+1].push_back(v);
            graph *g = new graph;
            g->enter = u;
            g->exit = v;
            a->y = g;
        }
        else
            a->y = NULL;
        regex.push_back(a);
    }
    a = new symbols;
    a->x = ')';
    a->y = NULL;
    regex.push_back(a);
    vector<int> stack;
    int start, stop;
    for(int i=0;i<regex.size();i++){
        if(regex[i]->x == '('){
            stack.push_back(i);
        }
        else if (regex[i]->x == ')' && stack.empty() == 0){
            start = stack.back();
            stack.pop_back();
            stop = i;
            vector <symbols *> newregex(regex.begin()+start+1,regex.begin()+stop);
            /*for(int j=0;j<newregex.size();j++)
                cout << newregex[j]->x;
            cout << endl << start << "\t" << stop << "\t" << endl;*/
            regex.erase(regex.begin()+start, regex.begin()+stop+1);
            graph *temp = removebrackets(newregex);
            symbols *a = new symbols;
            a->x = specialchar;
            a->y = temp;
            regex.insert(regex.begin()+start, a);
            i = start+1;
        }
        else if (regex[i]->x == ')'){
            cout << "Brackets do not match" << endl;
            return 0;
        }
    }
    if (stack.empty() == 0){
        cout << "Brackets do not match" << endl;
        return 0;
    }
    for(int i=0;i<final_table.size(); i++){
        cout << "From " << final_table[i]->id << endl;
        for(int j = 0; j<=noofsymbols;j++){
            if (j==0)
                cout << "\ton " << "epsilon" << " : ";
            else
                cout << "\ton " << char('a'+j-1) << " : ";
            for(int k = 0; k<final_table[i]->to[j].size();k++)
                cout << final_table[i]->to[j][k]->id << " ";
            cout << endl;
        }
    }
    cout << "You can start traversal from node " << regex[0]->y->enter->id << " and should reach node " << regex[0]->y->exit->id << endl;
    return 0;
}