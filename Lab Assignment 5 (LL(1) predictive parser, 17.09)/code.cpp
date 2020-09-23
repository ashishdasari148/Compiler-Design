#include <bits/stdc++.h>
//Include lexical analyser to get tokens sequentially from the input stream
#include "lex.yy.c"

using namespace std;

typedef struct symbols{
    string name;
    int id;
    bool is_terminal;
}symbols;

void eliminate_left_recursion(vector<symbols>& list_of_symbols, unordered_map<string,int>& name_to_id, unordered_map<int,vector<vector<int>>>& productions){
    //Modify the given grammar to an equivalent grammar that has only immediate left recursions but not any indirect left recursions
    for(int i=0;i<list_of_symbols.size();i++){
        if(!list_of_symbols[i].is_terminal){
            for(int j=0;j<i;j++){
                if(!list_of_symbols[j].is_terminal){
                    bool doer = false;
                    do{
                        doer = false;
                        for(auto k=productions[list_of_symbols[i].id].begin();k!=productions[list_of_symbols[i].id].end();k++){
                            if(k->at(0)==list_of_symbols[j].id){
                                vector<int> temp(k->begin(),k->end());
                                temp.erase(temp.begin());
                                productions[list_of_symbols[i].id].erase(k);
                                doer = true;
                                for(auto l=productions[list_of_symbols[j].id].begin();l!=productions[list_of_symbols[j].id].end();l++){
                                    vector<int> temp2;
                                    temp2.clear();
                                    temp2.insert(temp2.begin(), l->begin(), l->end());
                                    temp2.insert(temp2.end(), temp.begin(), temp.end());
                                    productions[list_of_symbols[i].id].push_back(temp2);
                                }
                                break;
                            }
                        }
                    }while(doer);
                }
            }
            //Find if there is immediate left recursion in the equivalent grammar!
            bool immediate_left = false;
            for(auto k=productions[list_of_symbols[i].id].begin();k!=productions[list_of_symbols[i].id].end();k++){
                if(k->at(0)==list_of_symbols[i].id){
                    immediate_left = true;
                }
            }
            if(immediate_left){
                //Add a new Non-Terminal to the Grammar (the same Non-Terminal with a ' at the end is used here!)
                symbols x;
                x.name = list_of_symbols[i].name + "'";
                x.id = list_of_symbols.size();
                x.is_terminal = false;
                name_to_id[list_of_symbols[i].name + "'"] = list_of_symbols.size();
                list_of_symbols.push_back(x);
                productions[name_to_id[list_of_symbols[i].name + "'"]] = vector<vector<int>>();
                for(auto k=productions[list_of_symbols[i].id].begin();k!=productions[list_of_symbols[i].id].end();k++){
                    if(k->at(0)==list_of_symbols[i].id){
                        k->erase(k->begin());
                        vector<int> temp(k->begin(),k->end());
                        temp.push_back(name_to_id[list_of_symbols[i].name + "'"]);
                        productions[name_to_id[list_of_symbols[i].name + "'"]].push_back(temp);
                        productions[list_of_symbols[i].id].erase(k);
                        k--;
                    }
                    else{
                        k->push_back(name_to_id[list_of_symbols[i].name + "'"]);
                    }
                }
                vector<int> temp;
                temp.push_back(name_to_id["epsilon"]);
                productions[name_to_id[list_of_symbols[i].name + "'"]].push_back(temp);
            }
        }
    }
    return;
}

void left_factor(vector<symbols>& list_of_symbols, unordered_map<string,int>& name_to_id, unordered_map<int,vector<vector<int>>>& productions){
    bool doer;
    do{
        doer = false;
        for(auto i=productions.begin();i!=productions.end();i++){
            for(auto j=i->second.begin();j!=i->second.end();j++){
                int maxmatch=0;
                for(auto k=j;k!=i->second.end();k++){
                    if(j==k) continue;
                    int place=0;
                    while(j->size()>place && k->size()>place && j->at(place)==k->at(place)){
                        place++;
                    }
                    if(place>maxmatch) maxmatch=place;
                }
                
                if(maxmatch!=0){
                    //Add a new symbol(Non-Terminal) to the grammar
                    symbols x;
                    //New Symbol generated due to left factoring is named as following to distinguish from new symbols created due to removal of left recursion
                    x.name = " " + to_string(list_of_symbols.size()) + " ";
                    x.id = list_of_symbols.size();
                    x.is_terminal = false;
                    name_to_id[" " + to_string(list_of_symbols.size()) + " "] = list_of_symbols.size();
                    list_of_symbols.push_back(x);
                    productions[list_of_symbols.size()-1] = vector<vector<int>>();
                    vector<int> temp(j->begin(),j->begin()+maxmatch);
                    temp.push_back(list_of_symbols.size()-1);
                    for(auto k=j;k!=i->second.end();k++){
                        int place=0;
                        while(place < temp.size()&&place < k->size()&&temp[place]==k->at(place)){
                            place++;
                        }
                        if(place==maxmatch){
                            vector<int> temp1(k->begin()+maxmatch, k->end());
                            if(temp1.size())
                                productions[list_of_symbols.size()-1].push_back(temp1);
                            else
                            {
                                temp1.push_back(name_to_id["epsilon"]);
                                productions[list_of_symbols.size()-1].push_back(temp1);
                            }
                            k--;
                            i->second.erase(k+1);
                            doer = true;
                        }
                    }
                    i->second.push_back(temp);
                    if(doer) break;
                }
                if(doer) break;
            }
            if(doer) break;
        }
    }while(doer);

    return;
}

void findfirst(vector<symbols>& list_of_symbols, int firstof, unordered_map<int,vector<vector<int>>>& productions, vector<unordered_set<int>> *first, unordered_map<string,int>& name_to_id){
    //if the symbol is a terminal
    if(list_of_symbols[firstof].is_terminal){
        first->at(firstof).insert(list_of_symbols[firstof].id);
        return;
    }
    //if the symbol is a non-terminal
    for(int i=0;i<productions[firstof].size();i++){
        unordered_set<int> tempset;
        tempset.insert(name_to_id["epsilon"]);
        for(int position = 0; position<productions[firstof][i].size()&&tempset.find(name_to_id["epsilon"])!=tempset.end();position++){
            tempset.erase(tempset.find(name_to_id["epsilon"]));
            //If first is not computed, recursively compute and use this to merge to the first set of firstof
            if(first->at(productions[firstof][i][position]).size()==0){
                findfirst(list_of_symbols, productions[firstof][i][position], productions, first, name_to_id);
            }
            tempset.insert(first->at(productions[firstof][i][position]).begin(), first->at(productions[firstof][i][position]).end());
        }
        first->at(firstof).insert(tempset.begin(), tempset.end());
    }
}

void compute_first_set(vector<symbols>& list_of_symbols, unordered_map<int,vector<vector<int>>>& productions, unordered_map<string,int>& name_to_id, vector<unordered_set<int>>& first){
    int first_size_prev[list_of_symbols.size()];
    memset(first_size_prev, 0, sizeof(first_size_prev));
    bool doer;
    do{
        doer = false;
        for(int i=0;i<list_of_symbols.size();i++){
            if(first[i].size()==0){
                if(list_of_symbols[i].is_terminal){
                    first[i].insert(list_of_symbols[i].id);
                    doer = true;
                    break;
                }
                findfirst(list_of_symbols, i, productions, &first, name_to_id);
                doer = true;
                break;
            }
            
        }
    }while(doer);

    return;
}

void compute_follow_set(vector<symbols>& list_of_symbols, unordered_map<int,vector<vector<int>>>& productions, unordered_map<string,int>& name_to_id, vector<unordered_set<int>>& follow, int start_i, vector<unordered_set<int>>& first){
    follow[start_i].insert(name_to_id["eof"]);
    int follow_size_prev[list_of_symbols.size()];
    memset(follow_size_prev, 0, sizeof(follow_size_prev));
    bool doer;
    do{
        doer = false;
        for(auto k=productions.begin();k!=productions.end();k++){
            for(int i=0;i<k->second.size();i++){
                for(int j=0;j<k->second[i].size();j++){
                    if(!list_of_symbols[k->second[i][j]].is_terminal){
                        unordered_set<int> tempset;
                        tempset.insert(name_to_id["epsilon"]);
                        for(int position = j+1; position<k->second[i].size()&&tempset.find(name_to_id["epsilon"])!=tempset.end();position++){
                            tempset.erase(tempset.find(name_to_id["epsilon"]));
                            tempset.insert(first[k->second[i][position]].begin(), first[k->second[i][position]].end());
                        }
                        if(tempset.find(name_to_id["epsilon"])!=tempset.end() || j+1>=k->second[i].size()){
                            tempset.erase(tempset.find(name_to_id["epsilon"]));
                            follow[k->second[i][j]].insert(follow[k->first].begin(),follow[k->first].end());
                        }
                        follow[k->second[i][j]].insert(tempset.begin(), tempset.end());
                    }
                }
            }
        }
        for(int i=0;i<list_of_symbols.size();i++){
            if(follow_size_prev[i]!=follow[i].size()){
                follow_size_prev[i] = follow[i].size();
                doer = true;
            }
        }
    }while(doer);
    return;
}

//Helper function to print the stack used in simulate function;
void stackprint(stack<int> s, vector<symbols> list_of_symbols){
    while(!s.empty()){
        cout << list_of_symbols[s.top()].name << " ";
        s.pop();
    }
    cout << endl;
    return;
}

bool build_parse_table(unordered_map<int,vector<vector<int>>>& productions, unordered_map<string,int>& name_to_id, vector<unordered_set<int>>& follow, vector<unordered_set<int>>& first, vector<vector<vector<int>*>>&parse_table){
    for(auto k=productions.begin();k!=productions.end();k++){
        for(int i=0;i<k->second.size();i++){
            unordered_set<int> tempset;
            tempset.insert(name_to_id["epsilon"]);
            for(int position = 0; position<k->second[i].size()&&tempset.find(name_to_id["epsilon"])!=tempset.end();position++){                
                tempset.erase(tempset.find(name_to_id["epsilon"]));
                tempset.insert(first[k->second[i][position]].begin(), first[k->second[i][position]].end());
            }
            if(tempset.find(name_to_id["epsilon"])!=tempset.end()){
                tempset.erase(tempset.find(name_to_id["epsilon"]));
                for(auto j=follow[k->first].begin();j!=follow[k->first].end();j++){
                    if(parse_table[k->first][*j]==NULL)
                        parse_table[k->first][*j] = &(k->second[i]);
                    else return false;
                }
            }
            for(auto j=tempset.begin();j!=tempset.end();j++){
                if(parse_table[k->first][*j]==NULL)
                    parse_table[k->first][*j] = &(k->second[i]);
                else return false;
            }
        }
    }
    return true;
}

bool simulate(vector<vector<vector<int>*>>&parse_table,vector<symbols>& list_of_symbols,unordered_map<string,int>& name_to_id,int start_i){
    stack<int>s;
    //Push eof(End Of File) and start symbol to the stack
    s.push(name_to_id["eof"]);
    s.push(start_i);
    int token=yylex();
    bool input_exhausted=false;
    while(s.top()!=name_to_id["eof"]||!input_exhausted){
        cout<<"Current Input Symbol:\t"<<list_of_symbols[token].name << "\t";
        stackprint(s, list_of_symbols);
        if(list_of_symbols[s.top()].is_terminal){
            if(s.top()==token){
                s.pop();
                if(token=yylex()){
                    if(token==0){
                        input_exhausted=true;
                        token=name_to_id["eof"];
                    }
                }
                else{
                    input_exhausted=true;
                    token=name_to_id["eof"];
                }
            }
            else{
                if(s.top()==name_to_id["epsilon"]){
                    s.pop();
                }
                else{
                    cout<<"ERROR_STACK_TOP_TERMINAL__INPUT_NO_MATCH"<<endl;
                    return false;
                }
            }
        }
        else{
            if(parse_table[s.top()][token]==NULL){
                //cout<<token<<" "<<list_of_symbols[token].name<<endl;
                cout<<"ERROR_PARSE_TABLE_ENTRY_NULL"<<endl;
                return false;
            }
            else{
                int t=s.top();s.pop();
                vector<int>* it=parse_table[t][token];
                int n=(int)(*it).size();
                // cout<<list_of_symbols[t].name<<" replaced with ";
                // for(int i=0;i<n;i++) cout<<list_of_symbols[(*it)[i]].name<<" ";cout<<"on top of stack"<<endl;
                for(int i=n-1;i>=0;i--){
                    s.push((*it)[i]);
                }
            }
        }
    }
    if(s.top()==name_to_id["eof"]&&input_exhausted) return true;
    //cout<<s.size()<<" "<<input_exhausted<<endl;
    return false;
}

int main(int argc, char* argv[]) {
    if(argc!=2&&argc!=3){
        cout<<"Insufficient Arguments to the executable\n"<<endl;
        return 0;
    }
    ifstream grammar;
    grammar.open(argv[1],ios::in);
    //Generate the lexical analyser from given lex file
    int number_of_symbols;
    grammar >> number_of_symbols;
    vector<symbols> list_of_symbols;
    unordered_map<string,int> name_to_id;
    for(int i=0;i<number_of_symbols;i++){
        string temp;
        int is_terminal;
        grammar >> temp;
        grammar >> is_terminal;
        name_to_id[temp] = i;
        symbols x;
        x.name = temp;
        x.id = i;
        x.is_terminal = is_terminal?true:false;
        list_of_symbols.push_back(x);
    }
    //Add epsilon to the grammar as a terminal
    symbols x;
    x.name = "epsilon";
    x.id = list_of_symbols.size();
    x.is_terminal = true;
    name_to_id["epsilon"] = list_of_symbols.size();
    list_of_symbols.push_back(x);
    //Add eof(End of File) to the grammar as a terminal
    x.name = "eof";
    x.id = list_of_symbols.size();
    x.is_terminal = true;
    name_to_id["eof"] = list_of_symbols.size();
    list_of_symbols.push_back(x);

    int number_of_productions;
    unordered_map<int,vector<vector<int>>> productions;
    int number_of_or_parts;
    int number_of_symbols_per_part;
    grammar >> number_of_productions;
    for(int i=0;i<number_of_productions;i++){
        string temp;
        grammar >> temp;
        productions[name_to_id[temp]] = vector<vector<int>>();
        grammar >> number_of_or_parts;
        for(int j=0;j<number_of_or_parts;j++){
            grammar >> number_of_symbols_per_part;
            productions[name_to_id[temp]].push_back(vector<int>());
            for(int k=0;k<number_of_symbols_per_part;k++){
                string symbols_of_part;
                grammar >> symbols_of_part;
                productions[name_to_id[temp]][j].push_back(name_to_id[symbols_of_part]);
            }
        }
    }
    //Print the grammar given by the user!
    cout << "Given Grammar is :" << endl;
    for(auto i=productions.begin();i!=productions.end();i++){
        cout << list_of_symbols[i->first].name << " -> ";
        for(int j=0;j<i->second.size();j++){
            for(int k=0;k<i->second[j].size();k++){
                cout << list_of_symbols[i->second[j][k]].name;
            }
            if(j!=i->second.size()-1)
                cout << " | ";
        }
        cout << endl;
    }
    //Eliminating Left Recursion!
    eliminate_left_recursion(list_of_symbols, name_to_id, productions);
    //Print the Grammar after eliminating Left Recursion
    cout << endl << "After Eliminating Left Recursion: " << endl;
    for(auto i=productions.begin();i!=productions.end();i++){
        cout << list_of_symbols[i->first].name << " -> ";
        for(int j=0;j<i->second.size();j++){
            for(int k=0;k<i->second[j].size();k++){
                cout << list_of_symbols[i->second[j][k]].name;
            }
            if(j!=i->second.size()-1)
                cout << " | ";
        }
        cout << endl;
    }
    //Left Factoring the Grammar!
    left_factor(list_of_symbols, name_to_id, productions);
    //Print Left Factored Grammar
    cout << endl << "Equivalent Left Factored Grammer:" << endl;
    for(auto i=productions.begin();i!=productions.end();i++){
        cout << list_of_symbols[i->first].name << " -> ";
        for(int j=0;j<i->second.size();j++){
            for(int k=0;k<i->second[j].size();k++){
                cout << list_of_symbols[i->second[j][k]].name;
            }
            if(j!=i->second.size()-1)
                cout << " | ";
        }
        cout << endl;
    }
    //If there is an empty production, Parse Tree can't be generated. So We do not take input as well and exit the program
    for(auto i=productions.begin();i!=productions.end();i++){
        if(i->second.size()==0){
            cout << "Since there are empty productions, You can't generate a parse tree for any given input (of tokens)" << endl;
            return 0;
        }
    }
    string start_s;
    int start_i;
    //cout << "Specify the start symbol: ";
    grammar >> start_s;
    start_i = name_to_id[start_s];
    vector<unordered_set<int>> first(list_of_symbols.size(), unordered_set<int>());
    vector<unordered_set<int>> follow(list_of_symbols.size(), unordered_set<int>());
    //Finding First set of all symbols in the grammar
    compute_first_set(list_of_symbols, productions, name_to_id, first);
    
    //Printing First of all symbols in the grammar
    for(int i=0;i<list_of_symbols.size();i++){
        cout << endl << "FIRST(" << list_of_symbols[i].name << ") : ";
        for(auto j=first[i].begin();j!=first[i].end();j++){
            cout << list_of_symbols[*j].name << " ";
        }
    }
    cout << endl;
    //Computing Follow set of Non-Terminals
    compute_follow_set(list_of_symbols, productions, name_to_id, follow, start_i, first);
    
    //Printing Follow set of all Non-Terminals
    for(int i=0;i<list_of_symbols.size();i++){
        if(list_of_symbols[i].is_terminal)
            continue;
        cout << endl << "FOLLOW(" << list_of_symbols[i].name << ") : ";
        for(auto j=follow[i].begin();j!=follow[i].end();j++){
            cout << list_of_symbols[*j].name << " ";
        }
    }
    cout << endl;

    //Constructing a Parse Table for the modified grammar
    vector<vector<vector<int>*>>parse_table(list_of_symbols.size(),vector<vector<int>*>(list_of_symbols.size(),NULL));
    bool is_LL1 = build_parse_table(productions, name_to_id, follow, first, parse_table);
    if(!is_LL1){
        cout << "The Grammar is not LL(1)... \n";
        return 0;
    }
    //Print the Parse Table
    //Parse Table is printed row wise seperately
    cout << "\nHere is the parse table... ";
    for(int i=0;i<list_of_symbols.size();i++){
        if(!list_of_symbols[i].is_terminal){
            cout << endl << "Row of " << list_of_symbols[i].name << ": " << endl;
            for(int j=0;j<list_of_symbols.size();j++){
                if(list_of_symbols[j].is_terminal){
                    if(parse_table[i][j]!=NULL){
                        cout << list_of_symbols[j].name << ": ";
                        for(auto k=parse_table[i][j]->begin();k!=parse_table[i][j]->end();k++){
                            cout << list_of_symbols[*k].name << " ";
                        }
                        cout << endl;
                    }
                }
            }
        }
    }
    if(argc==2){
        //Take input file for simulation!
        cout<<"Enter the input file name(with extension) to simulate and check\nFile Name : ";
        string file__name;
        cin>>file__name;
        yyin=fopen(file__name.c_str(),"r");
    }
    else yyin=fopen(argv[2],"r");
    cout<<"Checking the syntax of input file...\n\n";
    bool ans=simulate(parse_table,list_of_symbols,name_to_id,start_i);
    if(ans) cout<< "\n[ACCEPTED] Your input syntax is correct...\n";
    else cout << "\n[REJECTED] Your input syntax is wrong...\n";
    fclose(yyin);
    cout<<"Exiting...\n";
    return 0;
}
