#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if(!(cin>>n)) return 0;
    vector<int> finalOrder(n);
    for(int &x: finalOrder) cin>>x;

    // Current arrangement and position helper
    vector<int> track(n);
    iota(track.begin(), track.end(), 1);   // 1,2,...,n
    vector<int> pos(n+1);                  // pos[car] = index on track (0-based)
    for(int i=0;i<n;++i) pos[track[i]] = i;

    vector<pair<int,int>> overtakes;       // answer

    // Place cars one-by-one from left to right
    for(int i=0;i<n;++i){
        int target = finalOrder[i];
        // Move the target car leftwards until it reaches position i
        while(pos[target] > i){
            int idx = pos[target];          // idx > i
            int behind = track[idx];       // car that will overtake
            int front  = track[idx-1];     // car that will be overtaken
            overtakes.emplace_back(behind, front);
            swap(track[idx], track[idx-1]);
            // update positions (behind moved left, front moved right)
            pos[behind]--; 
            pos[front]++;
        }
    }

    cout << overtakes.size() << '\n';
    for(auto &e : overtakes)
        cout << e.first << ' ' << e.second << '\n';
    return 0;
}