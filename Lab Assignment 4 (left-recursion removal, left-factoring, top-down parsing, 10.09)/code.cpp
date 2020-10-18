#include <bits/stdc++.h>
using namespace std;
typedef struct symbols{
    string name;
    int id;
    bool is_terminal;
}symbols;
void traverse(bool *reached, int next_to_consume, vector<int> tokens, vector<int> leaves_of_parse_tree, vector<symbols>list_of_symbols, unordered_map<int,vector<vector<int>>> productions, vector<vector<int>> *tree, unordered_map<string,int> name_to_id){
    int first_non_terminal = -1;
    bool finished = true;
    if(leaves_of_parse_tree.size()<tokens.size())
        finished=false;
    for(int i=next_to_consume;i<leaves_of_parse_tree.size();i++){
        if(!list_of_symbols[leaves_of_parse_tree[i]].is_terminal&&first_non_terminal==-1){
            first_non_terminal = i;
            finished = false;
        }
        if(first_non_terminal==-1&&list_of_symbols[leaves_of_parse_tree[i]].is_terminal){
            if(i<tokens.size()){
                if(leaves_of_parse_tree[i]!=tokens[i]){
                    return;
                }
            }
            else
            {
                return;
            }
        }
    }
    if(finished){
        *reached = true;
        tree->push_back(leaves_of_parse_tree);
        return;
    }
    else if(first_non_terminal==-1){
        return;
    }
    for(int j=0;j<productions[leaves_of_parse_tree[first_non_terminal]].size()&&!(*reached);j++){
        vector<int> temp(leaves_of_parse_tree.begin(),leaves_of_parse_tree.end());
        temp.erase(temp.begin()+first_non_terminal);
        if(productions[leaves_of_parse_tree[first_non_terminal]][j][0]!=name_to_id["epsilon"])
            temp.insert(temp.begin()+first_non_terminal, productions[leaves_of_parse_tree[first_non_terminal]][j].begin(), productions[leaves_of_parse_tree[first_non_terminal]][j].end());
        traverse(reached,first_non_terminal,tokens,temp, list_of_symbols, productions, tree, name_to_id);
        if(*reached){
            tree->push_back(leaves_of_parse_tree);
            return;
        }
    }
}
int main(){
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
    cout << endl << "Now I dare you to give right tokens to parse." << endl << "Enter no of Tokens && Start Symbol" << endl;
    int no_of_tokens;
    string temp;
    vector<vector<int>> tree;
    vector<int> leaves_of_parse_tree;
    cin >> no_of_tokens;
    cin >> temp;
    leaves_of_parse_tree.push_back(name_to_id[temp]);
    vector<int>tokens;
    while(no_of_tokens--){
        cin >> temp;
        tokens.push_back(name_to_id[temp]);
    }
    bool reached = false;
    traverse(&reached, 0, tokens, leaves_of_parse_tree, list_of_symbols, productions, &tree, name_to_id);
    if(reached==true){
        cout << endl << "Here is your parse tree" << endl;
        for(int i=tree.size()-1;i>=0;i--){
            for(int j=0;j<tree[i].size();j++){
                cout << list_of_symbols[tree[i][j]].name << " ";
            }
            cout << endl;
        }
    }
    else{
        cout << endl << "Hahaha... You lost your dare, There is no second chance in this..." << endl;
    }
    return 0;
}