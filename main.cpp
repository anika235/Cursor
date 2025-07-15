#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; if(!(cin>>t)) return 0;
    while(t--){
        int n; cin>>n;
        vector<int> x(n);
        for(int i=0;i<n;++i) cin>>x[i];
        vector<int> y(n);
        for(int i=0;i<n;++i) cin>>y[i];
        char ans; cin>>ans; // 'A' or 'B'
        cout<<ans<<"\n";
    }
    return 0;
}