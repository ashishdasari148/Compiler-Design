#include <bits/stdc++.h>
#include "lex.yy.c"
using namespace std;
typedef struct symbols{
    string name;
    int id;
    bool is_terminal;
}symbols;
void findfirst(vector<symbols> list_of_symbols, int firstof, unordered_map<int,vector<vector<int>>> productions, vector<unordered_set<int>> *first, unordered_map<string,int> name_to_id){
    // cout << list_of_symbols[firstof].name << endl;
    if(list_of_symbols[firstof].is_terminal){
        first->at(firstof).insert(list_of_symbols[firstof].id);
        return;
    }
    for(int i=0;i<productions[firstof].size();i++){
        unordered_set<int> tempset;
        tempset.insert(name_to_id["epsilon"]);
        for(int position = 0; position<productions[firstof][i].size()&&tempset.find(name_to_id["epsilon"])!=tempset.end();position++){
            tempset.erase(tempset.find(name_to_id["epsilon"]));
            // cout << " First size() : " << first->at(productions[firstof][i][position]).size() << endl;
            if(first->at(productions[firstof][i][position]).size()==0){
                findfirst(list_of_symbols, productions[firstof][i][position], productions, first, name_to_id);
            }
            tempset.insert(first->at(productions[firstof][i][position]).begin(), first->at(productions[firstof][i][position]).end());
        }
        first->at(firstof).insert(tempset.begin(), tempset.end());
    }
    // cout << endl << list_of_symbols[firstof].name << " = ";
    // for(auto j=first->at(firstof).begin();j!=first->at(firstof).end();j++){
    //     cout << list_of_symbols[*j].name << " ";
    // }
}
int main() {
    int number_of_symbols;
    unordered_map<string,int> name_to_id;
    cin >> number_of_symbols;
    vector<symbols> list_of_symbols;
    for(int i=0;i<number_of_symbols;i++){
        string temp;
        int is_terminal;
        cin >> temp;
        cin >> is_terminal;
        name_to_id[temp] = i;
        symbols x;
        x.name = temp;
        x.id = i;
        x.is_terminal = is_terminal?true:false;
        list_of_symbols.push_back(x);
    }
    symbols x;
    x.name = "epsilon";
    x.id = list_of_symbols.size();
    x.is_terminal = true;
    name_to_id["epsilon"] = list_of_symbols.size();
    list_of_symbols.push_back(x);
    x.name = "eof";
    x.id = list_of_symbols.size();
    x.is_terminal = true;
    name_to_id["eof"] = list_of_symbols.size();
    list_of_symbols.push_back(x);
    // for(int i=0;i<number_of_symbols;i++){
    //     cout << list_of_symbols[i].name << " ";
    // }
    // cout << endl;
    int number_of_productions;
    unordered_map<int,vector<vector<int>>> productions;
    int number_of_or_parts;
    int number_of_symbols_per_part;
    cin >> number_of_productions;
    for(int i=0;i<number_of_productions;i++){
        string temp;
        cin >> temp;
        productions[name_to_id[temp]] = vector<vector<int>>();
        cin >> number_of_or_parts;
        for(int j=0;j<number_of_or_parts;j++){
            cin >> number_of_symbols_per_part;
            productions[name_to_id[temp]].push_back(vector<int>());
            for(int k=0;k<number_of_symbols_per_part;k++){
                string symbols_of_part;
                cin >> symbols_of_part;
                productions[name_to_id[temp]][j].push_back(name_to_id[symbols_of_part]);
            }
        }
    }
    cout << endl << "Given:" << endl;
    for(auto i=productions.begin();i!=productions.end();i++){
        cout << list_of_symbols[i->first].name << " = ";
        for(int j=0;j<i->second.size();j++){
            for(int k=0;k<i->second[j].size();k++){
                cout << list_of_symbols[i->second[j][k]].name;
            }
            if(j!=i->second.size()-1)
                cout << " | ";
        }
        cout << endl;
    }
    cout << endl;
    for(int i=0;i<number_of_symbols;i++){
        if(!list_of_symbols[i].is_terminal){
            for(int j=0;j<i;j++){
                if(!list_of_symbols[j].is_terminal){
                    bool doer = false;
                    do{
                        doer = false;
                        for(auto k=productions[list_of_symbols[i].id].begin();k!=productions[list_of_symbols[i].id].end();k++){
                            if(k->at(0)==list_of_symbols[j].id){
                                // cout << list_of_symbols[i].name << " " << list_of_symbols[j].name << endl;

                                // cout << list_of_symbols[i].name << " = ";
                                // for(int m=0;m<productions[i].size();m++){
                                //     for(int n=0;n<productions[i][m].size();n++){
                                //         cout << list_of_symbols[productions[i][m][n]].name;
                                //     }
                                //     if(m!=productions[i].size()-1)
                                //         cout << " | ";
                                // }
                                // cout << endl;
                                // cout << list_of_symbols[j].name << " = ";
                                // for(int m=0;m<productions[j].size();m++){
                                //     for(int n=0;n<productions[j][m].size();n++){
                                //         cout << list_of_symbols[productions[j][m][n]].name;
                                //     }
                                //     if(m!=productions[j].size()-1)
                                //         cout << " | ";
                                // }
                                // cout << endl;

                                vector<int> temp(k->begin(),k->end());
                                temp.erase(temp.begin());
                                productions[list_of_symbols[i].id].erase(k);
                                doer = true;
                                for(auto l=productions[list_of_symbols[j].id].begin();l!=productions[list_of_symbols[j].id].end();l++){
                                    vector<int> temp2;
                                    temp2.clear();
                                    temp2.insert(temp2.begin(), l->begin(), l->end());
                                    temp2.insert(temp2.end(), temp.begin(), temp.end());
                                    // for(int m=0;m<temp2.size();m++)
                                    //     cout << list_of_symbols[temp2[m]].name;
                                    // cout << endl;
                                    productions[list_of_symbols[i].id].push_back(temp2);
                                }
                                // cout << list_of_symbols[i].name << " = ";
                                // for(int m=0;m<productions[i].size();m++){
                                //     for(int n=0;n<productions[i][m].size();n++){
                                //         cout << list_of_symbols[productions[i][m][n]].name;
                                //     }
                                //     if(m!=productions[i].size()-1)
                                //         cout << " | ";
                                // }
                                // cout << endl;
                                break;
                            }
                        }
                    }while(doer);
                }
            }
            bool immediate_left = false;
            for(auto k=productions[list_of_symbols[i].id].begin();k!=productions[list_of_symbols[i].id].end();k++){
                if(k->at(0)==list_of_symbols[i].id){
                    immediate_left = true;
                }
            }
            if(immediate_left){
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
    cout << endl << "After Eliminating Left Recursion:" << endl;
    for(auto i=productions.begin();i!=productions.end();i++){
        cout << list_of_symbols[i->first].name << " = ";
        for(int j=0;j<i->second.size();j++){
            for(int k=0;k<i->second[j].size();k++){
                cout << list_of_symbols[i->second[j][k]].name;
            }
            if(j!=i->second.size()-1)
                cout << " | ";
        }
        cout << endl;
    }
    cout << endl;
    bool doer;
    do{
        doer = false;
        for(auto i=productions.begin();i!=productions.end();i++){
            for(auto j=i->second.begin();j!=i->second.end();j++){
                int maxmatch=0;
                for(auto k=j;k!=i->second.end();k++){
                    if(j==k)
                        continue;
                    int place=0;
                    
                    while(j->size()>place && k->size()>place && j->at(place)==k->at(place)){
                        // cout << list_of_symbols[j->at(place)].name << " " << list_of_symbols[k->at(place)].name << endl;
                        place++;
                    }
                    if(place>maxmatch)
                        maxmatch=place;
                }
                
                if(maxmatch!=0){
                    // cout << "Maxmatch: " << maxmatch << endl;
                    symbols x;
                    x.name = " " + to_string(list_of_symbols.size()) + " ";
                    x.id = list_of_symbols.size();
                    x.is_terminal = false;
                    name_to_id[" " + to_string(list_of_symbols.size()) + " "] = list_of_symbols.size();
                    list_of_symbols.push_back(x);
                    productions[list_of_symbols.size()-1] = vector<vector<int>>();
                    // vector<vector<int>> tempproduction;
                    vector<int> temp(j->begin(),j->begin()+maxmatch);
                    temp.push_back(list_of_symbols.size()-1);
                    // cout << "J size() : " << j->size() << endl;
                    // cout << "here...\n";
                    for(auto k=j;k!=i->second.end();k++){
                        int place=0;
                        // cout << "here''\n";
                        // cout << k->size() << endl;
                        // for(int n=0;n<k->size();n++)
                        //     cout << list_of_symbols[k->at(n)].name << " ";
                        while(place < temp.size()&&place < k->size()&&temp[place]==k->at(place)){
                            place++;
                        }
                        // cout << "Place: " << place << endl;
                        if(place==maxmatch){
                            vector<int> temp1(k->begin()+maxmatch, k->end());
                            if(temp1.size())
                                productions[list_of_symbols.size()-1].push_back(temp1);
                            else
                            {
                                temp1.push_back(name_to_id["epsilon"]);
                                productions[list_of_symbols.size()-1].push_back(temp1);
                            }
                            
                            // tempproduction.push_back(temp1);
                            k--;
                            // if(k==j)
                            //     j--;
                            i->second.erase(k+1);
                            doer = true;
                        }
                    }
                    // productions[list_of_symbols.size()-1] = tempproduction;
                    i->second.push_back(temp);
                    if(doer)
                        break;
                }
                if(doer)
                    break;
            }
            if(doer)
                break;
        }
    }while(doer);
    cout << endl << "Equivalent Left Factored Grammer:" << endl;
    for(auto i=productions.begin();i!=productions.end();i++){
        cout << list_of_symbols[i->first].name << " = ";
        for(int j=0;j<i->second.size();j++){
            for(int k=0;k<i->second[j].size();k++){
                cout << list_of_symbols[i->second[j][k]].name;
            }
            if(j!=i->second.size()-1)
                cout << " | ";
        }
        cout << endl;
    }
    for(auto i=productions.begin();i!=productions.end();i++){
        if(i->second.size()==0){
            cout << endl << "Since there are empty productions, You can't proceed further" << endl;
            return 0;
        }
    }
    string start_s;
    int start_i;
    cout << "Specify the start symbol: ";
    cin >> start_s;
    start_i = name_to_id[start_s];
    vector<unordered_set<int>> first(list_of_symbols.size(), unordered_set<int>());
    vector<unordered_set<int>> follow(list_of_symbols.size(), unordered_set<int>());
    int first_size_prev[list_of_symbols.size()];
    memset(first_size_prev, 0, sizeof(first_size_prev));
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
                // cout << " First size() : " << first[productions[i][0][0]].size() << endl;
                // break;
            }
            
        }
    }while(doer);
    for(int i=0;i<list_of_symbols.size();i++){
        cout << endl << "FIRST(" << list_of_symbols[i].name << ") : ";
        for(auto j=first[i].begin();j!=first[i].end();j++){
            cout << list_of_symbols[*j].name << " ";
        }
    }
    cout << endl;
    follow[start_i].insert(name_to_id["eof"]);
    int follow_size_prev[list_of_symbols.size()];
    memset(follow_size_prev, 0, sizeof(follow_size_prev));
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
    for(int i=0;i<list_of_symbols.size();i++){
        if(list_of_symbols[i].is_terminal)
            continue;
        cout << endl << "FOLLOW(" << list_of_symbols[i].name << ") : ";
        for(auto j=follow[i].begin();j!=follow[i].end();j++){
            cout << list_of_symbols[*j].name << " ";
        }
    }
    cout << endl;
    vector<vector<vector<int>*>>parse_table(list_of_symbols.size(),vector<vector<int>*>(list_of_symbols.size(),NULL));
    bool is_LL1 = true;
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
                    else
                        is_LL1 = false;
                }
            }
            for(auto j=tempset.begin();j!=tempset.end();j++){
                if(parse_table[k->first][*j]==NULL)
                    parse_table[k->first][*j] = &(k->second[i]);
                else
                    is_LL1 = false;
            }
        }
    }
    if(!is_LL1){
        cout << "The Grammar is not LL(1)... \n";
        return 0;
    }
    cout << "\nHere is your parse table";
    for(int i=0;i<list_of_symbols.size();i++){
        if(!list_of_symbols[i].is_terminal){
            cout << endl << "Entries of " << list_of_symbols[i].name << ": " << endl;
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
    cout << endl;
    vector<int> tokens;
    int token;
    yyin=fopen("inputfile.txt","r");
    while ((token=yylex())!=0){
        cout << token << "\t" << yytext << endl;
        tokens.push_back(token);
    }
    tokens.push_back(name_to_id["eof"]);
    for(int i=0;i<tokens.size();i++)
        cout << tokens[i] << " ";
    cout << endl;
    return 0;
}