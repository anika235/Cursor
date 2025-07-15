#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; if(!(cin>>t)) return 0;
    while(t--){
        int n; cin>>n;
        for(int i=0;i<n;++i){int tmp;cin>>tmp;} // read x
        for(int i=0;i<n;++i){int tmp;cin>>tmp;} // read y
        string ans; cin>>ans; // either "A" or "B"
        cout<<ans<<"\n";
    }
    return 0;
}