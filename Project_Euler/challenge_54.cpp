#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <chrono>

using namespace std;

map<char, int> get_value= {
        {'2', 2},
        {'3', 3},
        {'4', 4},
        {'5', 5},
        {'6', 6},
        {'7', 7},
        {'8', 8},
        {'9', 9},
        {'T', 10},
        {'J', 11},
        {'Q', 12},
        {'K', 13},
        {'A', 14}
    };

int get_level(vector<string> s){
    vector<string> unique_value_count;
    vector<string> unique_suits_count;
    unique_value_count.clear();
    unique_suits_count.clear();

    unique_copy(begin(s), end(s), back_inserter(unique_value_count), [](string a, string b){
        return a[0]==b[0];
    });
    unique_copy(begin(s), end(s), back_inserter(unique_suits_count), [](string a, string b){
        return a[1]==b[1];
    });
    int unique_values=unique_value_count.size();
    int unique_suits = unique_suits_count.size();

    if(unique_values==2){
        if(s[0][0]!=s[0][1])
            return 7;//Four of a kind
        else
        {
            return 6;//Full House
        }
        
    }
    else if(unique_suits==1){
        if(unique_values==5 && s[0][0] == 'T')
            return 9;// Royal Flush
        else if(unique_values==5 && get_value[s[4][0]]-get_value[s[0][0]]==4)
        {
            return 8;//Strait Flush
        }
        else 
        {
            return 5;//Flush
        }
    }
    else if(unique_values==5 && get_value[s[4][0]]-get_value[s[0][0]]==4)
        {
            return 4;//Straight
        }

    else if(unique_values==3)
    {
        if(((s[0][0]==s[1][0])&& (s[2][0]==s[1][0]))||((s[3][0]==s[1][0])&& (s[2][0]==s[1][0]))||((s[2][0]==s[3][0])&& (s[2][0]==s[4][0])))
            return 3;//three of a kind
        else 
            return 2;//2 pairs
    }
    else if(unique_values==4)
        return 1;//1 pair
    else
    {
        return 0;
    }
    }


int main(){
    auto start=chrono::high_resolution_clock::now();

    ifstream iFile;

    iFile.open("p054_poker.txt");

    if(!iFile)
    {
        cerr<<"file loading failed."<<endl;
        exit(1);
    }

    vector<vector<string> > player1(1000);
    vector<vector<string> > player2(1000);
    string x;

    vector<string> poker_1;
    vector<string> poker_2;
    map<char, int> get_value= {
        {'2', 2},
        {'3', 3},
        {'4', 4},
        {'5', 5},
        {'6', 6},
        {'7', 7},
        {'8', 8},
        {'9', 9},
        {'T', 10},
        {'J', 11},
        {'Q', 12},
        {'K', 13},
        {'A', 14}
    };

    int count=0;
    int point_p1=0, point_p2=0, p1_wins=0;
    
    while (iFile>>x){
        if(count<5){
            poker_1.push_back(x);
            count++;
        }
        else{
            poker_2.push_back(x);
            count++;
        }
        if(poker_1.size()<100)

        if(poker_1.size()==5 && poker_2.size()==5){
            sort(begin(poker_1), end(poker_1), [&get_value](string a, string b){
                return (get_value[a[0]]<get_value[b[0]]);             
            });

            sort(begin(poker_1), end(poker_1), [poker_1](string a, string b){
                int count_a=count_if(begin(poker_1), end(poker_1), [a](string t){
                    return t[0]==a[0];
                });

                int count_b=count_if(begin(poker_1), end(poker_1), [b](string t1){
                    return t1[0]==b[0];
                });    

                return count_a<count_b;   
            });

            sort(begin(poker_2), end(poker_2), [&get_value](string a, string b){
                return (get_value[a[0]]<get_value[b[0]]);             
            });

            sort(begin(poker_2), end(poker_2), [poker_2](string a, string b){
                int count_a=count_if(begin(poker_2), end(poker_2), [a](string t){
                    return t[0]==a[0];
                });

                int count_b=count_if(begin(poker_2), end(poker_2), [b](string t1){
                    return t1[0]==b[0];
                });    

                return count_a<count_b;   
            });

            if(get_level(poker_1)>get_level(poker_2)){
                p1_wins+=1;
            }
            else if(get_level(poker_1)==get_level(poker_2))
                p1_wins+=((get_value[poker_1[4][0]]>get_value[poker_2[4][0]]) ? 1:0);
          
            
            if(count%10==0){
                cout<<"player 1: ";
                for(int j=0;j<5;j++)
                    cout<<poker_1[j]<<" ";
                cout<<" :"<<get_level(poker_1)<<" player 2: ";
                for(int k=0;k<5;k++)
                    cout<<poker_2[k]<<" ";
                cout<<" :"<<get_level(poker_2)<<endl;}
                

            player1.push_back(poker_1);
            poker_1.clear();
            player2.push_back(poker_2);
            poker_2.clear();
        }
        count=count%10; 
        
    }
    cout<<"player 1 wins: "<<p1_wins<<endl;

    auto end=chrono::high_resolution_clock::now();
    cout<<"elapsed time: "<<(end-start).count()/1e9<<endl;
   // cout<<player1.size()<<" "<<player2.size()<<endl;




}