#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; if(!(cin>>n)) return 0;
    vector<int> finalOrder(n);
    for(int &x: finalOrder) cin>>x;

    // current track and helper arrays
    vector<int> track(n);
    iota(track.begin(), track.end(), 1);
    vector<int> pos(n+1);
    for(int i=0;i<n;++i) pos[track[i]] = i;

    // keep how many times a pair (smaller,bigger) or (any order) was used
    // using vector<unordered_map<int,int>> would be heavy, but n<=1000 so we can use matrix
    static uint8_t used[1001][1001] = {0};

    vector<pair<int,int>> overtakes; // answer

    auto addSwap=[&](int back,int front){
        overtakes.emplace_back(back,front);
        used[back][front]++;
        used[front][back]++;
    };

    // ----- Phase 1 : stable insertion sort (left -> right)
    for(int i=0;i<n;++i){
        int target = finalOrder[i];
        int p = pos[target];
        while(p>i){
            int back  = track[p];       // will overtake
            int front = track[p-1];     // being overtaken
            addSwap(back,front);
            swap(track[p],track[p-1]);
            pos[back]--; pos[front]++;
            --p;
        }
    }

    // At this point track == finalOrder

    // ----- Phase 2 : for every adjacent pair that still has 0 swaps possible (same initial order)
    for(int i=0;i+1<n;++i){
        int a = track[i];
        int b = track[i+1];
        if(a<b && used[a][b]==0){ // same relative order, still untouched
            // perform b overtake a, then a overtake b (two swaps) and restore order
            addSwap(b,a);
            swap(track[i],track[i+1]);
            addSwap(a,b);
            swap(track[i],track[i+1]);
        }
    }

    cout<<overtakes.size()<<"\n";
    for(auto &p:overtakes) cout<<p.first<<' '<<p.second<<"\n";
    return 0;
}