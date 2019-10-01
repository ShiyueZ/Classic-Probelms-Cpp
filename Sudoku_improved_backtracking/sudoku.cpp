#include<algorithm>
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<chrono>
#include<iterator>
#include <numeric> 

using namespace std;

class Sudoku
{
public:
    Sudoku(vector<vector<int> > sudoku_, vector<int> non_zeros_):s(sudoku_), non_zeros(non_zeros_){}
    vector<int> get_row(int x){
        return s[x];
    };

    vector<int> get_col(int y){
        vector<int> col(9);
        for(int i=0;i<9;i++){
            col[i]=s[i][y];
        }
        return col;
    };

    vector<int> get_grid(int x, int y){
        vector<int> grid(9);
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                grid[i*3+j]=s[x-x%3+i][y-y%3+j];
                }
        }
        return grid;
    };

    int get_R_count(int x){
        auto r=get_row(x);
        return count(begin(r), end(r), 0);
    }

    int get_C_count(int y){
        auto c=get_col(y);
        return count(begin(c), end(c), 0);
    }

    int get_G_count(int x, int y){
        auto g=get_grid(x, y);
        return count(begin(g), end(g), 0);
    }

    int get_nonZero(int x, int y){
        return (27-get_R_count(x)-get_C_count(y)-get_G_count(x, y));
    }

    auto get_weak_grid(){
        int j_=0, k_=0, max_nonZero=0;
        for(int k=0;k<9;k++){
            for(int j=0;j<9;j++){
                if(s[k][j]==0){
                    non_zeros[k*9+j]=get_nonZero(k, j);
                    if(non_zeros[k*9+j]>max_nonZero){
                        max_nonZero=non_zeros[k*9+j];
                        j_=j;k_=k;
                    }
                }
            }
        }
        if(max_nonZero!=0)
            return make_pair(k_, j_);
        else{
            cout<<"solved!"<<endl;
            return make_pair(-1, -1);
        }
        
    }

    vector<int> prune_current(pair<int,int> weak) {
        auto r=get_row(weak.first), c=get_col(weak.second), g=get_grid(weak.first, weak.second);
        transform(begin(c), end(c), back_inserter(r), [](int e){return e;});
        transform(begin(g), end(g), back_inserter(r), [](int e){return e;});

        sort(begin(r), end(r));
        r.erase(unique(begin(r), end(r)), end(r));

        vector<int> range(10), candidates;
        iota(begin(range), end(range), 0);
        set_difference(begin(range), end(range),begin(r), end(r), inserter(candidates, candidates.begin()));
        return candidates;
    }

    bool solve(){
        // copy s
        //auto s_tmp=s;
        auto weak=get_weak_grid();
        int ind_x=weak.first, ind_y=weak.second;
        if(ind_x==-1)
        {
            return true;
        }
        //cout<<"---solving---"<<endl;
        //cout<<"weak index: "<<ind_x<<", "<<ind_y<<endl;
        vector<int> candidates = prune_current(weak);
        
        /*cout<<"loop over "<<candidates.size()<<" candidates: ";
        for(int i=0;i<candidates.size(); i++)
            cout<<candidates[i]<<" ";
        cout<<endl;*/

        if(get_weak_grid()==make_pair(-1, -1)){
            return true;
        } else{
            for(int i=0;i<candidates.size(); i++){
                //cout<<"candidate: "<<candidates[i]<<endl;
                s[ind_x][ind_y]=candidates[i];
                vector<int> xs;
                vector<int> ys;
                bool feasible = true;
                bool changed = true;
                while (changed) {
                    changed = false;
                    if (!feasible) {
                        break;
                    }
                    for(int x=0;x<9;x++) {
                        if (!feasible) {
                            break;
                        }
                        for(int y=0;y<9;y++) {
                            if (s[x][y] == 0) {
                                vector<int> possible = prune_current(make_pair(x,y));
                                // no possible solution for this cell => infeasible => backtrack
                                if (possible.size() == 0) {
                                    s[ind_x][ind_y]=0;
                                    feasible = false;
                                    break;
                                }
                                // only a single possible solution => set it and rerun everything again
                                if (possible.size() == 1) {
                                    s[x][y]=possible[0];
                                    // save for resetting later back to 0 if infeasible
                                    xs.push_back(x);
                                    ys.push_back(y);
                                    changed = true;
                                    // break into while loop
                                    x = 9;
                                    y = 9;
                                }
                            }
                        }
                    }

                }

                if(feasible && solve()){
                    return true;
                }
                else {
                    // reset current cell and all pruned cells back to 0
                    s[ind_x][ind_y]=0;
                    for (int rev_idx=0; rev_idx<xs.size(); rev_idx++) {
                        s[xs[rev_idx]][ys[rev_idx]] = 0;
                    }
                }
               
                
            }
        }
        return false;        
    }

    void print(){
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                cout<<s[i][j]<<" ";
                }
            cout<<endl;
        }
    }

    ~Sudoku()=default;


private:
    vector<vector<int> > s;
    vector<int> non_zeros;
};


int main(){

    ifstream iFile("sudoku_hard.txt");

    if(!iFile)
    {
        cerr<<"file loading failed."<<endl;
        exit(1);
    }

    vector<vector<int> > sudoku(9);
    vector<int> nonZeros(81);
    string s_;
    int count=0;

    while(std::getline(iFile, s_)){
        istringstream ss(s_);
        int tmp;
        while(ss>>tmp){
            sudoku[count].push_back(tmp);
        }
        count++;
    }

    Sudoku su(sudoku, nonZeros);
    cout<<"original sudoku: "<<endl;
    su.print();

    auto start=chrono::high_resolution_clock::now();
    su.solve();
    auto end=chrono::high_resolution_clock::now();
    cout<<"elapsed time: "<<(end-start).count()/1e9<<" s"<<endl;
    cout<<"solution:"<<endl;
    su.print();




}
