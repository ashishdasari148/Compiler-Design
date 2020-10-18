#include <bits/stdc++.h>
//Include lexical analyser to get tokens sequentially from the input stream
#include "lex.yy.c"

using namespace std;

typedef struct symbols{
    string name;
    int id;
    bool is_terminal;
}symbols;

typedef struct transition{
    char trans_type;
    int to_state;
    int production_symbol;//symbol to the left of production
    int production_number;//a non terminal may have many productions, so this is the production no.
}transition;

typedef struct item{
    int production_symbol;//symbol to the left of production
    int production_number;//a non terminal may have many productions, so this is the production no.
    int dot_position;
    int end_terminal;
    bool operator< (const item& a)const{
        if(this->production_number!=a.production_number) return this->production_number<a.production_number;
        if(this->production_symbol!=a.production_symbol) return this->production_symbol<a.production_symbol;
        if(this->dot_position!=a.dot_position) return this->dot_position<a.dot_position;
        if(this->end_terminal!=a.end_terminal) return this->end_terminal<a.end_terminal;
        return false;
    }
}item;

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

item make_item(int production_symbol,int production_number,int dot_position,int end_terminal){
    item x;
    x.dot_position = dot_position;
    x.end_terminal = end_terminal;
    x.production_number = production_number;
    x.production_symbol = production_symbol;
    return x;
}

// set<item> compute_closure(set<item>&items, vector<symbols>&list_of_symbols, unordered_map<int,vector<vector<int>>>& productions, vector<unordered_set<int>>& first);

void generate_end_terminals(unordered_set<int>&end_terminals, std::set<item>::iterator it, unordered_map<int,vector<vector<int>>>& productions, vector<unordered_set<int>>& first, unordered_map<string,int>& name_to_id){
    int pos=(it->dot_position)+1;
    vector<int>* production=&productions[it->production_symbol][it->production_number];
    if(pos==production->size()){
        end_terminals.insert(it->end_terminal);
        return;
    }
    else{
        end_terminals=first[production->at(pos)];
        while(end_terminals.find(name_to_id["epsilon"])!=end_terminals.end()&&pos<(production->size())){
            end_terminals.erase(name_to_id["epsilon"]);
            pos++;
            if(pos==production->size()){
                end_terminals.insert(it->end_terminal);
                return;
            }
            end_terminals.insert(first[production->at(pos)].begin(),first[production->at(pos)].end());
        }
    }
    return;
}

set<item> compute_closure(set<item>&items, vector<symbols>&list_of_symbols, unordered_map<string,int>& name_to_id, unordered_map<int,vector<vector<int>>>& productions, vector<unordered_set<int>>& first){
    int prev_items_size=items.size();
    do{
        prev_items_size = items.size();
        for(auto it=items.begin();it!=items.end();it++){
            if(productions[it->production_symbol][it->production_number].size()==it->dot_position) continue;
            int curr_symbol=productions[it->production_symbol][it->production_number][it->dot_position];
            if(list_of_symbols[curr_symbol].is_terminal) continue;
            else{
                unordered_set<int>end_terminals;
                generate_end_terminals(end_terminals,it,productions,first,name_to_id);
                for(int i=0;i<productions[curr_symbol].size();i++){
                	if(productions[curr_symbol][i].size()==0) continue;
                    item new_item;
                    new_item.production_symbol=curr_symbol;
                    new_item.production_number=i;
                    if(productions[curr_symbol][i][0]==name_to_id["epsilon"]) new_item.dot_position=1;
                    else new_item.dot_position=0;
                    for(int end_terminal : end_terminals){
                        new_item.end_terminal = end_terminal;
                        items.insert(new_item);
                    }
                }
            }
        }
    }while(prev_items_size!=items.size());
    return items;
}

void print_item_set(set<item>&items, vector<symbols>&list_of_symbols, unordered_map<int,vector<vector<int>>>& productions, int node_number){
    cout << "State number: " << node_number << endl;
    for(auto it = items.begin();it!=items.end();it++){
        cout << list_of_symbols[it->production_symbol].name << " -> ";
        for(int i=0;i<productions[it->production_symbol][it->production_number].size();i++){
            if(i==it->dot_position)
                cout << ".";
            cout << list_of_symbols[productions[it->production_symbol][it->production_number][i]].name;
        }
        if(productions[it->production_symbol][it->production_number].size()==it->dot_position)
            cout << ".";
        cout << ", " << list_of_symbols[it->end_terminal].name << endl;
    }
    cout << endl;
}

bool build_parse_table(unordered_map<int,vector<vector<int>>>& productions, unordered_map<string,int>& name_to_id, vector<unordered_set<int>>& follow, vector<unordered_set<int>>& first, vector<vector<transition>>&parse_table, int start_i, vector<symbols> list_of_symbols){
    map<set<item>, int> nodes;
    set<item> items;
    transition x; x.trans_type = 0;
    items.insert(make_item(start_i, 0, 0,name_to_id["eof"]));
    items = compute_closure(items, list_of_symbols, name_to_id, productions, first);
    nodes[items] = nodes.size();
    print_item_set(items, list_of_symbols, productions, nodes[items]);
    parse_table.push_back(vector<transition>(list_of_symbols.size(), x));
    list<set<item>> q;
    q.push_back(items);
    while(!q.empty()){
        items = q.front();
        q.pop_front();
        vector<transition> temptransitions(list_of_symbols.size(), x);
        for(int i = 0; i<list_of_symbols.size(); i++){
            set<item> temp;
            for(auto it = items.begin(); it!=items.end();it++){
                if(it->dot_position != productions[it->production_symbol][it->production_number].size()){
                    if(productions[it->production_symbol][it->production_number][it->dot_position]==i){
                        temp.insert(make_item(it->production_symbol, it->production_number, it->dot_position+1, it->end_terminal));
                        if(temptransitions[i].trans_type == 0 || temptransitions[i].trans_type == 's'){
                            temptransitions[i].trans_type = 's';
                        }
                    }
                }
                else if(i==it->end_terminal && temptransitions[i].trans_type == 0){
                    if(i==name_to_id["eof"]&&it->production_symbol==start_i){
                        temptransitions[i].trans_type = 'a';
                        temptransitions[i].production_symbol = it->production_symbol; 
                        temptransitions[i].production_number = it->production_number;
                    }
                    else{
                        temptransitions[i].trans_type = 'r'; 
                        temptransitions[i].production_symbol = it->production_symbol; 
                        temptransitions[i].production_number = it->production_number;
                    }
                }
            }
            if(temp.size()==0)
                continue;
            temp = compute_closure(temp, list_of_symbols, name_to_id, productions, first);
            if(nodes.find(temp)==nodes.end()){
                nodes[temp]=nodes.size();
                parse_table.push_back(vector<transition>(list_of_symbols.size(), x));
                q.push_back(temp);
                print_item_set(temp, list_of_symbols, productions, nodes[temp]);
            }
            if(temptransitions[i].trans_type == 's')
                temptransitions[i].to_state = nodes[temp];
        }
        for(int i = 0; i<list_of_symbols.size(); i++){
            /*if((parse_table[nodes[items]][i].trans_type == 's' && temptransitions[i].trans_type == 'r')||
            (parse_table[nodes[items]][i].trans_type == 'r' && temptransitions[i].trans_type == 's')||
            (parse_table[nodes[items]][i].trans_type == 'r' && temptransitions[i].trans_type == 'r'))
                return false;
            else */if(parse_table[nodes[items]][i].trans_type == 0)
                parse_table[nodes[items]][i] = temptransitions[i];
            else
                return false;
            
        }
    }
    return true;
}

string return_production(unordered_map<int,vector<vector<int>>>& productions, vector<symbols> list_of_symbols, int prod_symbol, int prod_number){
    string x = list_of_symbols[prod_symbol].name + " -> ";
    for(int i=0;i<productions[prod_symbol][prod_number].size();i++){
        x += list_of_symbols[productions[prod_symbol][prod_number][i]].name;
    }
    return x;
}

void print_productions(unordered_map<int,vector<vector<int>>>& productions, vector<symbols> list_of_symbols){
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
}

//Helper function to print the stack used in simulate function;
void stackprint(stack<int> s){
    cout << "Stack: ";
    stack<int> x;
    while(!s.empty()){
        x.push(s.top());
        s.pop();
    }
    while(!x.empty()){
        cout << x.top() << " ";
        x.pop();
    }
    cout << endl;
    return;
}

bool simulate(vector<symbols> list_of_symbols, unordered_map<string,int>& name_to_id, unordered_map<int,vector<vector<int>>>& productions, vector<vector<transition>>&parse_table, int start_i){
    stack<int>s;
    s.push(0);
    int token=yylex();
    cout << "Start\t";
    while(true){
        cout << "Next Symbol:\t" << list_of_symbols[token].name << "\t" ;
        transition t=parse_table[s.top()][token];
        stackprint(s);
        if(t.trans_type=='s'){
            cout << "Shift\t";
            s.push(t.to_state);
            if(token=yylex()) {}
            else 
                token=name_to_id["eof"];
        }
        else if(t.trans_type=='r'){
            cout << "Reduce\t";
            int l=productions[t.production_symbol][t.production_number].size();
            if(productions[t.production_symbol][t.production_number][0]==name_to_id["epsilon"]) l=0;
            while(l--) s.pop();
            s.push(parse_table[s.top()][t.production_symbol].to_state);
        }
        else if(t.trans_type=='a'){
            cout << "Accept";
            return true;
        }
        else{
            cout<<"PARSE_TABLE_ENTRY_NULL_OR_INVALID"<<endl;
            break;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    if(argc!=2&&argc!=3){
        cout<<"Insufficient Arguments to the executable\n"<<endl;
        return 0;
    }
    ifstream grammar;
    grammar.open(argv[1],ios::in);
    
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

    grammar >> number_of_productions;
    for(int i=0;i<number_of_productions;i++){
        string temp;
        getline(grammar,temp);
        while(temp.size()==0){
        	getline(grammar,temp);
        }
        stringstream prod(temp);
        string p,t;
        prod>>p;
        productions[name_to_id[p]] = vector<vector<int>>();
        prod>>t;
        getline(prod,t,'|');
        string sp;
        while(t.size()){
        	stringstream sprod(t.substr(1));
        	productions[name_to_id[p]].push_back(vector<int>());
        	while(getline(sprod,sp,' ')){
        		productions[name_to_id[p]].back().push_back(name_to_id[sp]);
        	}
        	if(!getline(prod,t,'|')) break;
        }
    }
    //Print the grammar given by the user!
    cout << "Given Grammar is :" << endl;
    print_productions(productions, list_of_symbols);

    // //Eliminating Left Recursion!
    eliminate_left_recursion(list_of_symbols, name_to_id, productions);

    // //Print the Grammar after eliminating Left Recursion
    // cout << endl << "After Eliminating Left Recursion: " << endl;
    // print_productions(productions, list_of_symbols);

    // //Left Factoring the Grammar!
    left_factor(list_of_symbols, name_to_id, productions);

    // // Print Left Factored Grammar
    cout << endl << "Equivalent Left Factored Grammer:" << endl;
    print_productions(productions, list_of_symbols);

    //If there is an empty production, Parse Tree can't be generated. So We do not take input as well and exit the program
    for(auto i=productions.begin();i!=productions.end();i++){
        if(i->second.size()==0){
            cout << "Since there are empty productions, You can't generate a parse tree for any given input (of tokens)" << endl;
            return 0;
        }
    }
    string start_s;
    int start_i;
    grammar >> start_s;
    start_i = name_to_id[start_s];
    vector<unordered_set<int>> first(list_of_symbols.size(), unordered_set<int>());
    vector<unordered_set<int>> follow(list_of_symbols.size(), unordered_set<int>());
    
    //Finding First set of all symbols in the grammar
    compute_first_set(list_of_symbols, productions, name_to_id, first);
    // //Printing First of all symbols in the grammar
    // for(int i=0;i<list_of_symbols.size();i++){
    //     cout << endl << "FIRST(" << list_of_symbols[i].name << ") : ";
    //     for(auto j=first[i].begin();j!=first[i].end();j++){
    //         cout << list_of_symbols[*j].name << " ";
    //     }
    // }
    // cout << endl;

    //Computing Follow set of Non-Terminals
    compute_follow_set(list_of_symbols, productions, name_to_id, follow, start_i, first);
    
    // //Printing Follow set of all Non-Terminals
    // for(int i=0;i<list_of_symbols.size();i++){
    //     if(list_of_symbols[i].is_terminal)
    //         continue;
    //     cout << endl << "FOLLOW(" << list_of_symbols[i].name << ") : ";
    //     for(auto j=follow[i].begin();j!=follow[i].end();j++){
    //         cout << list_of_symbols[*j].name << " ";
    //     }
    // }
    // cout << endl << endl;

    cout << endl;

    //Constructing a Parse Table for the modified grammar
    vector<vector<transition>>parse_table;
    bool is_LR1 = build_parse_table(productions, name_to_id, follow, first, parse_table, start_i, list_of_symbols);
    if(!is_LR1){
        cout << "The Grammar is not LR(1)... \n";
        return 0;
    }

    //Print the Parse Table
    //Parse Table is printed row wise seperately
    cout << "\nHere is the parse table...\n";
    for(int i=0;i<parse_table.size();i++){
        cout << "=========================================================================" << endl;
        cout << "State " << i << ": " << endl;
        for(int j=0;j<list_of_symbols.size();j++){
            if(list_of_symbols[j].is_terminal && parse_table[i][j].trans_type == 's')
                cout << "\tACTION(s" << i << "," << list_of_symbols[j].name << ") = " << parse_table[i][j].trans_type << parse_table[i][j].to_state << endl; 
            else if(list_of_symbols[j].is_terminal && parse_table[i][j].trans_type == 'r')
                cout << "\tACTION(s" << i << "," << list_of_symbols[j].name << ") = " << parse_table[i][j].trans_type << ": " << return_production(productions, list_of_symbols, parse_table[i][j].production_symbol, parse_table[i][j].production_number) << endl; 
            else if(list_of_symbols[j].is_terminal && parse_table[i][j].trans_type == 'a')
                cout << "\tACTION(s" << i << "," << list_of_symbols[j].name << ") = accept" << endl;
        }
        cout << "-------------------------------------------------------------------------" << endl;
        for(int j=0;j<list_of_symbols.size();j++){
            if(!list_of_symbols[j].is_terminal && parse_table[i][j].trans_type == 's')
                cout << "\tGOTO(s" << i << "," << list_of_symbols[j].name << ") = " /*<< parse_table[i][j].trans_type*/ << parse_table[i][j].to_state << endl; 
            /*else if(!list_of_symbols[j].is_terminal && parse_table[i][j].trans_type == 'r')
                cout << "\tGOTO(s" << i << "," << list_of_symbols[j].name << ") = " << parse_table[i][j].trans_type << ": " << parse_table[i][j].production_symbol << " " << parse_table[i][j].production_number << endl; 
            else if(!list_of_symbols[j].is_terminal && parse_table[i][j].trans_type == 'a')
                cout << "\tGOTO(s" << i << "," << list_of_symbols[j].name << ") = accept" << endl;*/
        }
        cout << "=========================================================================" << endl;
    }
    cout << endl;
    yyin=fopen(argv[2],"r");
    cout<<"Checking the syntax of input file...\n\n";
    bool ans=simulate(list_of_symbols,name_to_id,productions,parse_table,start_i);
    if(ans) cout<< "\n[ACCEPTED] Your input syntax is correct...\n";
    else cout << "\n[REJECTED] Your input syntax is wrong...\n";
    fclose(yyin);
    cout<<"Exiting...\n";
    return 0;
}