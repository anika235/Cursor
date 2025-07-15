#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; if(!(cin>>t)) return 0;
    while(t--){
        int n; if(!(cin>>n)) return 0;
        vector<long long> x(n);
        for(int i=0;i<n;++i) cin>>x[i];

        // helper for flushing query and reading answer with error check
        auto ask=[&](int a,int b){
            cout << "? " << a << ' ' << b << endl;
            long long d; if(!(cin>>d) || d==-1) exit(0);
            return d;
        };

        bool isGraph=false;

        // 1. look for duplicate x values
        unordered_map<long long,int> pos;
        int dup1=-1, dup2=-1;
        for(int i=0;i<n;++i){
            auto it=pos.find(x[i]);
            if(it!=pos.end()){ dup1=it->second; dup2=i; break; }
            pos[x[i]]=i;
        }
        if(dup1!=-1){
            // duplicate found
            long long d1=ask(dup1+1, dup2+1);
            long long d2=ask(dup2+1, dup1+1);
            if(d1==0 || d2==0 || d1!=d2) isGraph=true;
            // else remains false (Object B)
        } else {
            // x all distinct
            if(n==3){
                long long d1=ask(1,2);
                long long d2=ask(1,3);
                if(d1==0 || d1==1 || d2==0 || d2==1) isGraph=true;
            } else { // n>=4
                int idxMin=0, idxMax=0;
                for(int i=1;i<n;++i){
                    if(x[i]<x[idxMin]) idxMin=i;
                    if(x[i]>x[idxMax]) idxMax=i;
                }
                long long d1=ask(idxMin+1, idxMax+1);
                if(d1==0 || d1==1){
                    isGraph=true;
                } else {
                    long long d2=ask(idxMax+1, idxMin+1);
                    if(d2==0 || d2==1 || d1!=d2) isGraph=true;
                    else if(d1==2 && d2==2) isGraph=true;
                }
            }
        }
        cout << "! " << (isGraph?'A':'B') << endl;
    }
    return 0;
}