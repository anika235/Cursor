#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; if(!(cin>>n)) return 0;
    vector<int> target(n);
    for(int &x: target) cin>>x;

    vector<pair<int,int>> ans;

    // Phase-1: start with ascending 1..n   -> bubble to strict descending n..1
    vector<int> line(n);
    iota(line.begin(), line.end(), 1);

    for(int i=1;i<n;++i){
        int j=i;
        while(j>0 && line[j] > line[j-1]){ // need bigger number to the left
            ans.emplace_back(line[j], line[j-1]); // rear line[j] overtakes
            swap(line[j], line[j-1]);
            --j;
        }
    }

    // Phase-2: transform descending order to required permutation
    // Build position array
    vector<int> pos(n+1);
    for(int i=0;i<n;++i) pos[line[i]] = i;

    for(int i=0;i<n;++i){
        int car = target[i];
        while(pos[car] > i){
            int idx = pos[car];              // car is at idx > i
            int front = line[idx-1];         // car immediately in front
            ans.emplace_back(car, front);    // car overtakes front
            swap(line[idx], line[idx-1]);
            // update positions
            pos[car]--;  pos[front]++;
        }
    }

    // Sanity: line must equal target
#ifdef LOCAL
    for(int i=0;i<n;++i) assert(line[i]==target[i]);
#endif
    cout<<ans.size()<<"\n";
    for(auto &p: ans) cout<<p.first<<' '<<p.second<<"\n";
    return 0;
}